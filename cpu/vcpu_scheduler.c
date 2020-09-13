#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <libvirt/libvirt.h>

void connectToVM(){
    virConnectPtr conn = virConnectOpen("qemu:///system");

    if (conn == NULL) {
        printf("Failed to open connection to qemu:///system \n");
        return;
    } 

    //Get all active running Virtual Machines    
     virDomainPtr *domains;
    int numberOfVCPUS;
    size_t i;
    unsigned int flags = VIR_CONNECT_LIST_DOMAINS_RUNNING | VIR_CONNECT_LIST_DOMAINS_PERSISTENT;
    numberOfVCPUS = virConnectListAllDomains(conn, &domains, flags);
    printf("\nNumber of domains returned:  %d\n", numberOfVCPUS);
    if(numberOfVCPUS<0){
        printf("Failed to get all domains\n");
        return;
    }

    // Get VCPU STATS
    long long int vcpuStartTimeArray[numberOfVCPUS];
    float vcpuTimeDiffArray[numberOfVCPUS];
    int vcpuTotalTime;
    time_t time1;
    time(&time1);
    for (i = 0; i < numberOfVCPUS; i++) {
        virDomainInfo virDomainInfo;
        virDomainGetInfo(domains[i], &virDomainInfo);
        vcpuStartTimeArray[i] = virDomainInfo.cpuTime;
    }

    sleep(5);
    
    time_t time2;
    time(&time2);
    for (i = 0; i < numberOfVCPUS; i++) {
        virDomainInfo virDomainInfo2;
        virDomainGetInfo(domains[i], &virDomainInfo2);
        vcpuTimeDiffArray[i] = (float)(virDomainInfo2.cpuTime - vcpuStartTimeArray[i])/1000000000;
    }



    float vcpuUsagePercent[numberOfVCPUS];
    for (i = 0; i< numberOfVCPUS; i++){
        if(vcpuTimeDiffArray[i] != 0){
            vcpuUsagePercent[i] = (float) vcpuTimeDiffArray[i]/difftime(time2, time1);
            printf("VCpu Usage Percentage:  %f\n", vcpuUsagePercent[i]);
        }else{
            vcpuUsagePercent[i] = 0;
        }
    }

    //Get PCPU STATS
    // // Number of PCPUs
    // int numberofPCPUs;
    // unsigned char * cpumap;
    // numberofPCPUs = virNodeGetCPUMap(conn, &cpumap, NULL, 0);
    // printf("Number of PCPUs available: %i\n", numberofPCPUs);

    // // PCU Usage Percentage

    // virNodeCPUStats virNodeCPUStatsPtr[4];
    // // virNodeCPUStats * virNodeCPUStatsPtr;
    // int nparams = 4;
    // int cpuNum = 0;

    // for(cpuNum = 0; cpuNum < numberofPCPUs; cpuNum++){
    //     // virNodeCPUStatsPtr = malloc(sizeof(virNodeCPUStats) * nparams);
    //     virNodeGetCPUStats(conn, cpuNum, virNodeCPUStatsPtr, &nparams, 0);

    //     printf("%s CPU time for %i:  %lli\n", virNodeCPUStatsPtr[0].field, cpuNum, virNodeCPUStatsPtr[0].value);
    // }



    // if (virNodeGetCPUStats(conn, cpuNum, NULL, &nparams, 0) == 0 &&
    //     nparams != 0) {
    //     if ((virNodeCPUStatsPtr = malloc(sizeof(virNodeCPUStats) * nparams)) == NULL)
    //         printf("Unable to allocte memory to the virNodeCPUStats Ptr\n");
    //     memset(virNodeCPUStatsPtr, 0, sizeof(virNodeCPUStats) * nparams);
    //     if (virNodeGetCPUStats(conn, cpuNum, virNodeCPUStatsPtr, &nparams, 0))
    //         printf("Unable to get virNodeGetCPUStats\n");
    // }30270230000000


    // for (i = 0; i < ret; i++) {
    //     int ncpumaps = 1;
    //     int maplen = 3;
    //     unsigned char * cpumaps = calloc(ncpumaps, maplen);
    //     int vcpuPinInfo_ret;
    //     vcpuPinInfo_ret = virDomainGetVcpuPinInfo(domains[i], ncpumaps, cpumaps, maplen, VIR_DOMAIN_AFFECT_CURRENT);
    //     if(vcpuPinInfo_ret<0){
    //         printf("Failed to get Vcpu Pin info\n");
    //         return;
    //     }

    //     int j;
    //     printf("Number of virtual cpus %d\n", vcpuPinInfo_ret);
    //     for ( j=0; j < maplen ; j++){
    //             printf("\tValue of bit %i is %u\n", j, cpumaps[1]);
    //     }
    //     free(cpumaps);



    //     // int	virDomainPinVcpu(virDomainPtr domain, unsigned int vcpu, unsigned char * cpumap, int maplen)
    // }

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

int main(int argc, char **argv){
    char *callName = argv[0];
    char *time = argv[1];

    printf("Kicking off %s with time value %s\n", callName, time);
    
    connectToVM();
    return 0;
}