# VM MEMORY MANAGER 

## Motivation
* The purpose of the VM Manager is to dynamically reclaim and allott memory to virtual machines using the libvirt-api
* Learn manipulating the hypervisor using the libvirt api
* Understand virtual machine usage metrics and how they can be tuned


## Problem Statement
* Given a set of 4 virtual machines with dynamic memory usage find an algorithm to reclaim memory from virtual machines that dont need it and allot memory to machines that do need it
* The Memory Manager must keep guest operating systems from starving out
* The Memory Manager must reclaim memory from guest operating systems that are idle
* The Memory Manager must also be careful not to starve the Host Operating System

## Solution
1. Gather pertinent stistics from the virtual machine using libvirt-api
	* VCPU Memory Statistics (Actual, Unused, Available Memory)
	* Host Operating System Memory Statistics (Free Memory in Host)
	* Calculate whether a VCPU's memory consumption is increasing or decreasing over time
2. Trim Memory
	* If a VCPU's Unused Memory exceeds 70MB, reduce the Allotted memory for that VCPU by 10% of its Actual Memory
3. Alott Memory
	* If a VCPU's memory consumption is increasing and its unused memory is less than 30% of its Actual memory, increase the alloted actual memory by 20%
	* Before alloting any memory to VCPUs confirm that the Host Machine has enough memory to spare

## Executing
1. Compile memory_coordinator.c using the given 'MakeFile'
```
cd ~/Daniel_Vaseekaran_p1/memory
make
```
2. Ensure the Virtual Machines are up and running
3. execute memory_coordinator by giving it a time argument
```
./memory_coordinator 12
```
4. Note that the vcpu_scheduler will run continously and will need to be stopped manually
