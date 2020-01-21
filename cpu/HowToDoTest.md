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
3. Open a new terminal (e.g., a separate terminal window or a tmux/screen session) and use the *script* command to capture terminal output by running command *script cpu_testcase1.log*
4. In the same terminal, start the provided monitoring tool by running the command *~/Project1/cpu/monitor.py*. The *script* command from Step 3 will capture the monitoring tool output to *cpu_testcase1.log* so that you can submit it with your assignment.
5. In a new terminal, start your CPU Scheduler by running the *vcpu_scheduler* binary
6. In a new terminal, start the first test case by running the command *~/Project1/cpu/runtest1.py*. Note that this command launches the test case binaries as subprocesses, so although the command exits almost immediately the tests will still be executing on your virtual machines.
7. Use the output from the monitoring tool to determine if your CPU Scheduler is producing the correct behavior as described under *~/Project1/cpu/testcases/1/README.md*
8. After the test has completed, stop *monitor.py* by typing "Ctrl+C" in its terminal and then type "exit" on the command line to stop *script* from logging the console output. A record of your monitoring will be in the file *cpu_testcase1.log*, which you will submit with your assignment deliverables.
9. Shut down your test VMs with the script *~/Project1/shutdownallvm.py*
10. Repeat these steps for the remaining test cases, substituting the test case number as appropriate.

