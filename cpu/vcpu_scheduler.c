#include <stdio.h>
#include <stdlib.h>
#include <libvirt/libvirt.h>

void connectToVM(){
    virConnectPtr conn = virConnectOpen("qemu:///system");

    if (conn == NULL) {
    printf("Failed to open connection to qemu:///system \n");
        return;
    } 

    // printf("%s \n", virConnectGetSysinfo(conn,0));

    //Get all active running virautl machines    //virConnectList*
    
    virDomainPtr *domains;
    int ret;
    size_t i;
    unsigned int flags = VIR_CONNECT_LIST_DOMAINS_RUNNING | VIR_CONNECT_LIST_DOMAINS_PERSISTENT;
    ret = virConnectListAllDomains(conn, &domains, flags);
    printf("\nNumber of domains returned:  %d\n", ret);
    if(ret<0){
        printf("Failed to get all domains\n");
        return;
    }

    int vcpuTimeArray[8];
    int vcpuTotalTime;
    for (i = 0; i < ret; i++) {
        virDomainInfo virDomainInfo;
        virDomainGetInfo(domains[i], &virDomainInfo);
        printf("\nDomain state: %d\n", virDomainInfo.state);
        printf("Domain maxMem: %ld\n", virDomainInfo.maxMem);
        printf("Domain memory: %ld\n", virDomainInfo.memory);
        printf("Domain nrVirtCpu: %d\n", virDomainInfo.nrVirtCpu);
        printf("Domain cpuTime: %lld\n", virDomainInfo.cpuTime);

        vcpuTimeArray[i] = virDomainInfo.cpuTime/1000000;
        vcpuTotalTime = vcpuTotalTime + virDomainInfo.cpuTime/1000000;
    }

    float vcpuUsagePercent[8];
    for (i = 0; i< ret; i++){
        vcpuUsagePercent[i] = (float) vcpuTimeArray[i]/vcpuTotalTime;
        printf("VCPU percentage for vcpu%li is %f\n%%", i, vcpuUsagePercent[i]);
    }



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

    for (i = 0; i < ret; i++) {
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