#!/usr/bin/env python

from __future__ import print_function

import sys
import zipfile
import os

projectJson = {
    "ProjectStructure": [
        "/cpu/vcpu_scheduler.c",
        "/cpu/Makefile",
        "/cpu/Readme",
        "/cpu/vcpu_scheduler1.log",
        "/cpu/vcpu_scheduler2.log",
        "/cpu/vcpu_scheduler3.log",
        "/cpu/vcpu_scheduler4.log",
        "/cpu/vcpu_scheduler5.log",
        "/memory/memory_coordinator.c",
        "/memory/Makefile",
        "/memory/Readme",
        "/memory/memory_coordinator1.log",
        "/memory/memory_coordinator2.log",
        "/memory/memory_coordinator3.log",
    ]
}

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: " + sys.argv[0] + " Daniel_Vaseekaran_p1.zip")
        exit(1)
    outFile = sys.argv[1]
    zipDir = os.path.splitext(os.path.basename(outFile))[0]
    thisdir = os.path.dirname(os.path.abspath(__file__))
    project_files = projectJson["ProjectStructure"]

    with zipfile.ZipFile(outFile, 'w') as zf:
        for item in project_files:
            if not os.path.exists(thisdir + item):
                print(thisdir + item, "missing from submission")
                exit(1)
            zf.write(thisdir + item, zipDir + item)

