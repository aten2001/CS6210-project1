# VM CPU SCHEDULER

## Motivation
* The purpose of this scheduler is to distribute guest cpu usage to the host operating system's cpus. 
* Learn manipulating the hypervisor using the libvirt api
* Understand CPU usage metrics and how they can be tuned


## Problem Statement
* Given a set of 8 virtual machines with dynamic usage find an algorithm to pin each virtual machines to a physical cpus based on the usage metrics gathered using libvirt's libvirt-domain and libvirt-host library's
* The virtual machine usage must be evenly distributed amongst available physical cpus
* Once a stable distribution is reached the the algorithm must not alter the cpu mapping

## Solution
1. Gather pertinent stistics from the virtual machine using libvirt-api
	* VCPU Usage Percentage (calculated by monitoring usage over a period of time)
	* VCPU to PCPU Mapping information
	* PCPU Usage statitics (calculated using VCPU Usage and CPU Mapping)
2. If the CPU mapping is in flux over time or if the PCPU Usage is not evenly distributed, update the CPU Mapping to drive towards a more stable environment

## Executing
1. Compile vcpu_scheduler.c using the given 'MakeFile'
```
cd ~/Daniel_Vaseekaran_p1/cpu
make
```
2. Ensure the Virtual Machines are up and running
3. execute vcpu_schduler by giving it a time argument
```
./vcpu_scheduler 12
```
4. Note tha vcpu_scheduler will run continously and will need to be stopped manually
