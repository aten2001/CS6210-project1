# Memory Coordinator

This directory contains files for evaluating your Memory Coordinator. There are 3 test cases in total, each of which introduces a different workload.

## Prerequisites

Before starting these tests, ensure that you have created at least 4 virtual machines. The names of these VMs **MUST** start with *aos_* (e.g., aos_vm1 for the first VM, aos_vm2 for the second VM, etc.). Note that if you have already created VMs as part of the CPU Scheduler tests, you can re-use 4 those VMs here but will need to delete the other 4. Otherwise, if you are starting with this portion of the assignment you will be creating 4 VMs here and another 4 VMs when you begin the CPU Scheduler portion.

If you need to delete VMs, you can do so with the command:

`uvt-kvm destroy aos_vm1`

(where 'aos_vm1' is the name of the VM you wish to destroy)

If you need to create VMs, you can do so with the command:

`uvt-kvm create aos_vm1 release=xenial --memory 512`

(where 'aos_vm1' is the name of the VM you wish to create)

Ensure the VMs are shutdown before starting. You can do this with the script *~/Project1/shutdownallvm.py*

Compile the test programs for the Memory Coordinator evaluations with the following commands:

```
$ cd ~/Project1/memory/
$ ./makeall.sh
```
 
## Running The Tests

For each test, you will need to follow the procedure outlined below:

(Assuming testcase 1 as an example)
1. Set the maximum allowed memory for VMs by running the script *~/Project1/setallmaxmemory.py*
2. Start all of the VMs using the *~/Project1/startallvm.py* script
3. Copy the test binaries into each VM by running the *~/Project1/memory/assignall.sh* script
4. Start the provided monitoring tool in a new terminal session (e.g., a separate terminal window or a tmux/screen session) by running the script *~/Project1/memory/monitor.py*
5. In a new terminal, start your Memory Coordinator by running the *memory_coordinator* binary
6. In a new terminal, start the first test case by running the script *~/Project1/memory/runtest1.py*. Note that this script launches the test case binaries as subprocesses, so although the script exits almost immediately the tests will still be executing on your virtual machines.
7. Use the output from the monitoring tool to determine if your Memory Coordinator is producing the correct behavior as described under *~/Project1/memory/testcases/1/README.md*
8. After the test has completed, shut down your test VMs with the script *~/Project1/shutdownallvm.py*
9. Repeat these steps for the remaining test cases, substituting the test case number as appropriate.

