#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h> //sleep
#include <stdbool.h> 
#include <libvirt/libvirt.h>



void connectToVM(virConnectPtr *conn){
    *conn = virConnectOpen("qemu:///system");

    if (*conn == NULL) {
        printf("Failed to open connection to qemu:///system \n");
        return;
    } 
}

void getDomainInfo(virConnectPtr *conn, virDomainPtr **domains, int *numberOfVCPUS){
    // GET VM DOMAINS  
    size_t i;
    unsigned int flags = VIR_CONNECT_LIST_DOMAINS_RUNNING | VIR_CONNECT_LIST_DOMAINS_PERSISTENT;
    *numberOfVCPUS = virConnectListAllDomains(*conn, domains, flags);
    printf("--------------BEGIN ITERATION----------------\nNumber of domains available:  %d\n", *numberOfVCPUS);
    if(*numberOfVCPUS<0){
        printf("Failed to get all domains\n");
        return;
    }
}

virNodeMemoryStatsPtr getNodeMemoryStats(virConnectPtr *conn){
    // int	virNodeGetMemoryStats		(virConnectPtr conn,int cellNum,virNodeMemoryStatsPtr params,int * nparams,unsigned int flags);
    int nparams = 4;
    virNodeMemoryStatsPtr params = calloc(nparams, sizeof(virNodeMemoryStats));
    if (virNodeGetMemoryStats(*conn, -1, params, &nparams, 0))
        printf("virNodeGetMemoryStats Failed!\n");

    return params;
}

void setMemory(virDomainPtr *domains, int vcpuIndex, int amountOfMemory){
    if(virDomainSetMemory(domains[vcpuIndex], amountOfMemory) < 0){
        printf("Error:  Memory set failed!");
    };
}

void getVCPUMemoryStats(virDomainPtr *domains, int *numberOfVCPUS, virConnectPtr *conn){
    printf("\n-----------------------\nStep 1:  Get Memory Stats for VCPU\n");

    char memStatTags[10][15] ={"SWAP_IN", "SWAP_OUT", "MAJOR_FAULT", "MINOR_FAULT", "UNUSED", "AVAILABLE", "ACTUAL_BALLOON", "RSS", "USABLE", "LAST_UPDATE"};
    bool isvcpuMemUsageInc[*numberOfVCPUS];
    virDomainMemoryStatPtr stats_begin_array[*numberOfVCPUS];
    virDomainMemoryStatPtr stats_end_array[*numberOfVCPUS];
    int nr_stats = 9;
    int actualMemIndex = 0;
    int unusedIndex = 0;


    int i=0;
    for (i = 0; i < *numberOfVCPUS; i++){
        virDomainMemoryStatPtr stats_begin = calloc(nr_stats, sizeof(virDomainMemoryStatStruct));
        int period = 1;
        virDomainSetMemoryStatsPeriod(domains[i], period, 1);
        int ret = virDomainMemoryStats(domains[i], stats_begin, nr_stats, 0);
        stats_begin_array[i] = stats_begin;
    }

    sleep(3);
    
    for (i = 0; i< *numberOfVCPUS; i++){
        virDomainMemoryStatPtr stats_end = calloc(nr_stats, sizeof(virDomainMemoryStatStruct));
        int ret = virDomainMemoryStats(domains[i], stats_end, nr_stats, 0);

        int j;
        for(j=0; j<nr_stats; j++){
            if(stats_end[j].tag == 6){
                actualMemIndex=j;
            }
            if(stats_end[j].tag == 4){
                unusedIndex=j;
            }
        }

        printf("Unused begin:  %lli, end:  %lli, diff: %i\n", stats_begin_array[i][unusedIndex].val, stats_end[unusedIndex].val, stats_end[unusedIndex].val < stats_begin_array[i][unusedIndex].val);
        if(stats_end[unusedIndex].val < stats_begin_array[i][unusedIndex].val){ //Comparing Unused Memory
            isvcpuMemUsageInc[i] = true;
        }else{
            isvcpuMemUsageInc[i] = false;
        }
        stats_end_array[i] = stats_end;
    }

    //free stats    
    virNodeMemoryStatsPtr params = getNodeMemoryStats(conn); 
    printf("\n---------------------\nStep 2: Get Node Memory Stats:\n\n");    
    for (i = 0; i < 4; i++){
        printf("Node Memory %s:  %lli\n", params[i].field, params[i].value);
    }

    printf("\n---------------------\nStep 3: Print all stats!:\n\n");
    for(i = 0; i < *numberOfVCPUS; i++){
        printf("VCPU %i---\n", i);
        printf("\tMemory Usage Increasing:  %d\n", isvcpuMemUsageInc[i]);
        printf("\tActual Memory (kibi):  %lli\n", stats_end_array[i][actualMemIndex].val);
        printf("\tUnUsed Memory (kb):  %lli\n", stats_end_array[i][unusedIndex].val);
    }
    printf("\nFree Memory in Host:  %lli\n", params[1].value);

    printf("\n---------------------\nStep 4: Trim Memory:\n\n");
    for(i = 0; i< *numberOfVCPUS; i++){
        if(!isvcpuMemUsageInc[i]){
            if(stats_end_array[i][unusedIndex].val - stats_end_array[i][actualMemIndex].val * 0.1 > 70000){
                printf("Trim Memory for %i to %f\n", i, stats_end_array[i][actualMemIndex].val * 0.9);
                setMemory(domains, i, stats_end_array[i][actualMemIndex].val * 0.9);
            }
        }
    }

    printf("\n---------------------\nStep 5: Allocate Memory:\n\n");
    for (i=0; i<*numberOfVCPUS; i++){
        if(isvcpuMemUsageInc[i]){
            if(stats_end_array[i][unusedIndex].val < stats_end_array[i][actualMemIndex].val * 0.3){
                printf("Increase memory for %i to %f\n", i, stats_end_array[i][actualMemIndex].val * 1.2);
                if(params[1].value * 0.8 > stats_end_array[i][actualMemIndex].val * 1.2){
                    setMemory(domains, i, stats_end_array[i][actualMemIndex].val * 1.2);
                }
            }
        }
    }

    for (i = 0; i<*numberOfVCPUS; i++){
        free(stats_begin_array[i]);
        free(stats_end_array[i]);
    }
}

void cleanup(virConnectPtr *conn, virDomainPtr *domains, int *numberOfVCPUS){
    int i = 0;
    for (i = 0; i < *numberOfVCPUS; i++) {
        virDomainFree(domains[i]);
    }
    free(domains);
    virConnectClose(*conn);
    return;
}

void runMemoryCoordinator(){
    virConnectPtr conn;
    virDomainPtr *domains;
    int numberOfVCPUS;

    connectToVM(&conn);
    getDomainInfo(&conn, &domains, &numberOfVCPUS);
    getVCPUMemoryStats(domains, &numberOfVCPUS, &conn);

    cleanup(&conn, domains, &numberOfVCPUS);
}

int main(int argc, char **argv){
    char *callName = argv[0];
    int time = atoi(argv[1]);
    printf("\n---------------------------------------------\n");
    printf("Kicking off %s every %d seconds\n", callName, time);
    printf("---------------------------------------------\n");

    while(1){
        runMemoryCoordinator();

        if (time > 4){
            sleep(time - 3); //to account for 3 sec sleep during getVCPUMemoryStats() method
        }else{
            sleep(1); //Run every second if timer is set to less than 3 seconds to account for 3 second sleep
        }
    }

    return 0;
}