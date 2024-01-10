import time 
import os
while True:
 time.sleep(1)
 print("still running")
 print("pid is "+str(os.getpid()))
 i=os.sched_getscheduler(0)
 if i==os.SCHED_OTHER:
  print('sched_other');
 if i==os.SCHED_FIFO:
  print('sched_fifo');
 if i==os.SCHED_RR:
  print('sched_rr');

