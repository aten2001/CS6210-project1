#!/usr/bin/python
from vm import VMManager
from testLibrary import TestLib
from __future__ import print_function
import sys

global VM_PREFIX = "aos"

def main(args):
    if len(args) != 1:
        print('Usage: ./assignfiletoallvm.py [file]')
        exit(-1)
    filename = sys.argv[1]
    manager = VMManager()
    vms = manager.getRunningVMNames()
    filteredVms = [vm for vm in vms if vm.startswith(VM_PREFIX)]
    ips = TestLib.getIps(filteredVms)
    TestLib.copyFiles(filename,ips)

if __name__ == '__main__':
    main(sys.argv[1:])
