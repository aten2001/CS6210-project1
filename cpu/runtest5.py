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
    pinTuple = manager.getPinTupleToOneCpu(0)
    for vmname in vms:
        manager.pinVCpuToPCpu(vmname,0,pinTuple)
    ips = TestLib.getIps(vms)
    ipsAndVals = dict()
    i = 0
    for ip in ips:
        if i%2 == 0: 
            # Process running at 100%
            ipAndVals[ip]=[250000]
        else:
            # Process running at 50%
            ipAndVals[ip]=[30000]
        i+=1
    TestLib.startTestCase("~/cpu/testcases/5/iambusy {}",ipsAndVals)
