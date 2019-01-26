### Configuration

1. Create 8 virtual machines.
2. Put their name in vmlist.conf
3. Shutdown all of them

### Run testcases

1. Run startallvm.py
2. Run makeall.sh
3. Run assignall.sh
4. Run your monitor tool ( a simple monitor tool provided [monitor.py]. You are free to use other tools too).
5. Run runtest1.py (Note: using subprocess, experiment continues after main script exits.)
6. Run killall.py to kill the processes in each vm.

### Cleanup
1. Run shutdownallvm.py or destroyallvm.py 
2. Run cleanall.py

### Repeat the same steps for other test cases( runtest2.py, runtest3.py, runtest4.py, runtest5.py, one by one.)
### Refer README.md under testcases/<x> for more details on testcases. 
