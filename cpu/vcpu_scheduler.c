#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h> //sleep
#include <libvirt/libvirt.h>

float stdevOverMean(float data[], int array_size) {
    float sum = 0.0, mean, SD = 0.0;
    int i;
    for (i = 0; i < array_size; ++i) {
        sum += data[i];
    }
    mean = sum / array_size;
    for (i = 0; i < array_size; ++i)
        SD += pow(data[i] - mean, 2);
    SD = SD/array_size;
    SD = sqrt(SD);
    printf("\tSTANDARD DEVIATION:  %f\n", SD);
    printf("\tMEAN:  %f\n", mean);

    return SD/mean;
}

float averagePcpuUsage(float data[], int numberofVCPUs, int numberOfPCPUS){
    float sum = 0.0, mean;
    int i;
    for (i = 0; i < numberofVCPUs; ++i) {
        sum += data[i];
    }
    mean = sum / numberOfPCPUS;
    return mean;
}

int findLowest(float data[], int array_size){
    float currentLowest = data[0];
    int currentLowestIndex = 0;
    int i;
    for (i=0; i<array_size; i++){
        if(data[i] < currentLowest){
            currentLowest = data[i];
            currentLowestIndex = i;
        }
    }    
    return currentLowestIndex;
}

void connectToVM(){
    virConnectPtr conn = virConnectOpen("qemu:///system");

    if (conn == NULL) {
        printf("Failed to open connection to qemu:///system \n");
        return;
    } 

// GET VM DOMAINS  
     virDomainPtr *domains;
    int numberOfVCPUS;
    size_t i;
    unsigned int flags = VIR_CONNECT_LIST_DOMAINS_RUNNING | VIR_CONNECT_LIST_DOMAINS_PERSISTENT;
    numberOfVCPUS = virConnectListAllDomains(conn, &domains, flags);
    printf("\nNumber of domains (1 vcpu/domain) available:  %d\n", numberOfVCPUS);
    if(numberOfVCPUS<0){
        printf("Failed to get all domains\n");
        return;
    }

// Get VCPU STATS
    // Find VCPU Usage Percentage
    long long int vcpuStartTimeArray[numberOfVCPUS];
    float vcpuTimeDiffArray[numberOfVCPUS];
    int vcpuTotalTime;
    time_t time1;
    time(&time1);
    for (i = 0; i < numberOfVCPUS; i++) {
        virVcpuInfo virVcpuInfo;
        virDomainGetVcpus(domains[i], &virVcpuInfo, 1, NULL, 0);
        vcpuStartTimeArray[i] = virVcpuInfo.cpuTime;
    }
    
    sleep(3);

    time_t time2;
    time(&time2);
    for (i = 0; i < numberOfVCPUS; i++) {
        virVcpuInfo virVcpuInfo2;
        virDomainGetVcpus(domains[i], &virVcpuInfo2, 1, NULL, 0);
        vcpuTimeDiffArray[i] = (float)(virVcpuInfo2.cpuTime - vcpuStartTimeArray[i])/1000000000;
    }

    float vcpuUsagePercent[numberOfVCPUS];
    for (i = 0; i < numberOfVCPUS; i++){
        if(vcpuTimeDiffArray[i] != 0){
            vcpuUsagePercent[i] = (float) vcpuTimeDiffArray[i]/difftime(time2, time1);
            printf("VCpu Usage Percentage:  %f\n", vcpuUsagePercent[i]);
        }else{
            vcpuUsagePercent[i] = 0;
        }
    }

//GET PCPU STATS
    // Number of PCPUs
    int numberOfPCPUS;
    unsigned char * cpumap;
    numberOfPCPUS = virNodeGetCPUMap(conn, &cpumap, NULL, 0);
    printf("\nNumber of PCPUs available: %i\n", numberOfPCPUS);

    //PCPU Usage Percentage
    float pcpuUsagePercentage[numberOfPCPUS];
    for (i=0; i<numberOfPCPUS; i++) pcpuUsagePercentage[i] = 0;
    
    for (i = 0; i < numberOfVCPUS; i++) {
        int ncpumaps = 1;  //matches number of vcpus in domain
        int maplen = VIR_CPU_MAPLEN(numberOfPCPUS); 
        unsigned char * cpumaps = calloc(ncpumaps, maplen);
        int vcpuPinInfo_ret;

        vcpuPinInfo_ret = virDomainGetVcpuPinInfo(domains[i], ncpumaps, cpumaps, maplen, VIR_DOMAIN_AFFECT_CURRENT);
        if(vcpuPinInfo_ret<0){
            printf("Failed to get Vcpu Pin info\n");
            return;
        }

        int j=0;
        for(j=0; j<numberOfPCPUS; j++){
            if(VIR_CPU_USED(cpumaps, j)){
                pcpuUsagePercentage[j] = pcpuUsagePercentage[j] + vcpuUsagePercent[i];
            }
        }

        free(cpumaps);
    }

    for (i=0; i<numberOfPCPUS; i++){
        printf("Pcpu %li Usage Percentage:  %f\n", i, pcpuUsagePercentage[i]);
    }

    // PCPU Balance Stats
    float stdevOverMean_pcpuUsagePercentage = stdevOverMean(pcpuUsagePercentage, numberOfPCPUS);
    printf("Standard Deviation over mean for pcpu Usage:  %f\n", stdevOverMean_pcpuUsagePercentage);

// REPIN CPU LOGIC
    printf("Logic started\n");
    int pinAllocationArray[numberOfVCPUS];
    for (i = 0; i< numberOfVCPUS; i++){
        pinAllocationArray[i] = -1;
    }

    if(stdevOverMean_pcpuUsagePercentage > 1){
        if(numberOfVCPUS <= numberOfPCPUS){
            // 1:1 mapping if numberOfVCPUS <= numberOfPCPUS
            for(i = 0; i < numberOfVCPUS; i++){
                pinAllocationArray[i] = i;
            }
        }else{
            float pcpuProjectedPercentage[numberOfPCPUS];
            float idealpcpuPercentage = averagePcpuUsage(vcpuUsagePercent, numberOfVCPUS, numberOfPCPUS);

            for(i=0; i<numberOfVCPUS; i++){
                if(pinAllocationArray[i] == 0){
                    int j;
                    for(j=0; j<numberOfPCPUS;j++){
                        if(pcpuProjectedPercentage[j] + vcpuUsagePercent[i] < idealpcpuPercentage){
                            pinAllocationArray[i] = j;
                            break;
                        }
                    }		
                }
            }

            for(i = 0; i<numberOfVCPUS; i++){
                if(pinAllocationArray[i] == -1){
                    int j = findLowest(pcpuProjectedPercentage, numberOfPCPUS);
                    pinAllocationArray[i] = j;
                }
            }
        }
    }

    printf("Logic completed\n");


// IMPLEMENT PINS
    for(i = 0; i < numberOfVCPUS; i++){
        printf("Implementing Pins for VCPU: %li\n", i);
        int ncpumaps = 1;
        int maplen = VIR_CPU_MAPLEN(numberOfPCPUS); 
        unsigned char * cpumaps = calloc(ncpumaps, maplen);
        int j;
        for(j = 0; j < numberOfPCPUS; j++){
            if(j == pinAllocationArray[i]){
                VIR_USE_CPU(cpumaps, j);
                printf("\tChecking if PCPU %i is set to USE\n", j);
            }
        }
        int vcpu_pin_ret;
        vcpu_pin_ret = virDomainPinVcpu(domains[i], 0, cpumaps, maplen);
        if(vcpu_pin_ret < 0){
            printf("Error when printing VCPU to CPU\n");
        }
    }

// CLEAN UP
    for (i = 0; i < numberOfVCPUS; i++) {
        virDomainFree(domains[i]);
    }
    free(domains);


    //Collect VCPU statistics using libvirt-host virDomainGet*
    //If you need pcpu information checkout libvirt-host api

    //How do you transform vcpu time from nanoseconds to % form

    //Determine the current map (affinity) between VCPU to PCPU through virDomainGet* functions

    //Write an algorithm to find the best PCPU to pin each VCPU

    //Revise "one-time scheduler" to run periodically

    //Test
    virConnectClose(conn);
    return;
}

void testEverything(){
    printf("/**************TESTING**************/\n\n");
    float testData1[] = {8, 7, 9, 1, 24, 25};
    float testData2[] = {8, 7, 9, 1, 24, 25};
    float testData3[] = {8, 7, 9, 1, 24, 25};
    float testData4[] = {8, 7, 9, 1, 24, 25};
    float testData5[] = {8, 7, 9, 1, 24, 25};


    //Size Test
    size_t array_size = sizeof(testData)/sizeof(testData[0]);

    printf("Array Size:  %ld\n", array_size);


    //meanTest
    printf("Ideal CPU Usage:  %f\n", averagePcpuUsage(testData, array_size, 12));


    //stdevTest
    printf("StandardDev/Mean:  %f\n", stdevOverMean(testData, array_size));
    
    //findLowestTest
    printf("Lowest Index:  %i\n", findLowest(testData, array_size));
    printf("\n\n/********TESTING COMPLETE********/\n\n");


}



int main(int argc, char **argv){
    char *callName = argv[0];
    char *time = argv[1];

    testEverything();

    printf("Kicking off %s with time value %s\n", callName, time);
    
    // connectToVM();
    return 0;
}