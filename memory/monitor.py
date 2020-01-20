#!/usr/bin/python
from __future__ import print_function
import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from vm import VMManager
from testLibrary import TestLib
import sched, time

VM_PREFIX="aos"

def run(sc,vmobjlist,memoryinfolist):
    i = 0
    for vm in vmobjlist:
        memoryinfolist[i] = vm.memoryStats()
        print("memory,{},{},{}"
            .format(vm.name(), 
                    memoryinfolist[i]['actual'] / 1024.0,
                    memoryinfolist[i]['unused'] / 1024.0))
        i+=1
    sc.enter(2, 1, run, (sc,vmobjlist,memoryinfolist))

if __name__ == '__main__':
    s = sched.scheduler(time.time, time.sleep)
    manager = VMManager()
    vmlist = manager.getRunningVMNames(VM_PREFIX)
    vmobjlist = [manager.getVmObject(name) for name in vmlist]   
    
    for vm in vmobjlist:
        vm.setMemoryStatsPeriod(1)   
 
    memoryinfolist = [None] * len(vmobjlist)

    s.enter(2, 1, run, (s,vmobjlist,memoryinfolist))
    s.run()
