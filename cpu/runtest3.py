#!/usr/bin/python
from __future__ import print_function
import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from vm import VMManager
from TestLibrary import TestLib

VM_PREFIX="aos"

if __name__ == '__main__':
    manager = VMManager()
    vms = manager.getRunningVMNames(VM_PREFIX)
    rounds = manager.getPhysicalCpus()
    i=0
    for vmname in vms:
        pinList = manager.getPinTupleToOneCpu(i)
        manager.pinVCpuToPCpu(vmname,0,pinList)
        i = (i + 1) % rounds
    ips = TestLib.getIps(vms)
    ipsAndVals = { ips : [100000] for ip in ips }
    TestLib.startTestCase("~/cpu/testcases/3/iambusy {}",ipsAndVals)
