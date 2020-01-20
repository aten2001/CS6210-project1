#!/usr/bin/python

from __future__ import print_function
import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from vm import VMManager
from TestLibrary import TestLib
import sched, time

VM_PREFIX="aos"

def which_cpu(vcpuinfo):
    return vcpuinfo[0][0][3]

def which_usage(newinfo, oldinfo):
    return ( newinfo[0][0][2] - oldinfo[0][0][2]) * 1.0 / (10 ** 9)

def run(sc,numpcpu,vmlist,vmobjlist,vminfolist):
    for i in range(numpcpu):
        cpulist[i] = {}
        cpulist[i]['mapping'] = []
        cpulist[i]['usage'] = 0.0

    for i in range(len(vmobjlist)):
        newinfo = vmobjlist[i].vcpus()
        if vminfolist[i]:
            cpu = which_cpu(newinfo)
            usage = which_usage(newinfo, vminfolist[i])
            cpulist[cpu]['mapping'].append(vmlist[i])
            cpulist[cpu]['usage'] += usage
        vminfolist[i] = newinfo
    
    for i in range(numpcpu):
        print('usage,{},{}'.format(i,cpulist[i]['usage'] * 100))
        for mapping in cpulist[i]['mapping']:
            print('mapping,{},{}'.format(i,mapping))

    s.enter(1, 1, run, (s,numcpu,vmlist,vmobjlist,vminfolist,))

if __name__ == '__main__':
    s = sched.scheduler(time.time, time.sleep)
    manager=VMManager()
    vmlist = manager.getRunningVMNames(VM_PREFIX)
    vmobjlist = [manager.getVmObject(name) for name in vmlist]   
    vminfolist = [None] * len(vmobjlist)
    numpcpu = manager.getPhysicalCpus() 
    
    s.enter(1, 1, run, (s,numcpu,vmlist,vmobjlist,vminfolist))
    s.run()
