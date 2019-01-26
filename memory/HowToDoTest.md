### Configuration

1. Create 4 virtual machines.
2. Put their name in vmlist.conf
3. Shutdown all of them and run setallmaxmemory.py

### Running testcases

1. Run startallvm.py
2. Run makeall.sh
3. Run assignall.sh
4. Run your monitor tool(a simple provided [ monitor.py], Others are okay too).
5. Run runtest1.py (Note: using subprocess, experiment continues after main script exits)
 

### Cleanup
1. Run shutdownallvm.py or destroyallvm.py 

### Repeat the above steps for  testcase 2 and 3 
### Please refer to README.md under testcase/<x> for more ionformation on testcases.

