# CPU Scheduler

This directory contains files for evaluating your CPU Scheduler. There are 5 test cases in total, each of which introduces a different workload.

## Prerequisites

Before starting these tests, ensure that you have created 8 virtual machines. The names of these VMs **MUST** start with *aos_* (e.g., aos_vm1 for the first VM, aos_vm2 for the second VM, etc.).

If you need to create VMs, you can do so with the command:

`uvt-kvm create aos_vm1 release=xenial --memory 256`

(where 'aos_vm1' is the name of the VM you wish to create)

Note that if you started with the Memory Coordinator portion of this assignment you should already have 4 VMs in place and as such will need to create 4 additional VMs using the above command. Otherwise if you are starting from this portion of the assignment you will be creating 8 VMs.

Ensure the VMs are shutdown before starting. You can do this with the script *~/Project1/shutdownallvm.py*

Compile the test programs for the CPU Scheduler evaluations with the following commands:

```
$ cd ~/Project1/cpu/
$ ./makeall.sh
```
 
## Running The Tests

For each test, you will need to follow the procedure outlined below:

(Assuming testcase 1 as an example)
1. Start all of the VMs using the *~/Project1/startallvm.py* script
2. Copy the test binaries into each VM by running the *~/Project1/cpu/assignall.sh* script
3. Start the provided monitoring tool in a new terminal session (e.g., a separate terminal window or a tmux/screen session) by running the script *~/Project1/cpu/monitor.py*
4. In a new terminal, start your CPU Scheduler by running the *vcpu_scheduler* binary
5. In a new terminal, start the first test case by running the script *~/Project1/cpu/runtest1.py*. Note that this script launches the test case binaries as subprocesses, so although the script exits almost immediately the tests will still be executing on your virtual machines.
6. Use the output from the monitoring tool to determine if your CPU Scheduler is producing the correct behavior as described under *~/Project1/cpu/testcases/1/README.md*
7. After the test has completed, shut down your test VMs with the script *~/Project1/shutdownallvm.py*
8. Repeat these steps for the remaining test cases, substituting the test case number as appropriate.

