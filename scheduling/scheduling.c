
/* Example taken from http://www.cs.cofc.edu/~leclerc/340/examples/schedtest.c*/

#include <stdio.h>        // for printf()
#include <sched.h>        // for sched_xxx() function calls
#include <sys/types.h>    // for pid_t
#include <sys/time.h>     // for getpriority()
#include <sys/resource.h> // for getpriority()
#include <unistd.h>       // for nice()

int main(int argc, char** argv)
{
  int schedpolicy;
  int prio;
  int minprio, maxprio;
  struct sched_param p;
  int rrinterval;
  struct timespec ts;

  p.sched_priority = 1;

  if (sched_setscheduler((pid_t) 0, SCHED_RR, &p) == -1) {
    printf("couldn't set scheduler to SCHED_RR\n");
  }

  if (sched_rr_get_interval((pid_t) 0, &ts) != -1) {
    printf("quantum = %ld secs %ld nanosecs\n", ts.tv_sec, ts.tv_nsec);
  }

  schedpolicy = sched_getscheduler((pid_t) 0);  // i.e. this process

  switch (schedpolicy) {
    case SCHED_FIFO:
      printf("SCHED_FIFO\n");
      break;
    case SCHED_RR:
      printf("SCHED_RR\n");
      break;
    case SCHED_OTHER:
      printf("SCHED_OTHER\n");
      break;
    default:
      printf("unknown scheduling policy\n");
  }

  // get and print priority of this process
  prio = getpriority(PRIO_PROCESS, 0);
  printf("process priority is %d\n", prio);

  // NOTE:  only root can increase the priority of a process
  nice(10);   // i.e. lower the priority of this process

  // get and print priority of this process
  prio = getpriority(PRIO_PROCESS, 0);
  printf("process priority is %d\n", prio);

  // get minimum and maximum priority for this scheduling policy
  // NOTE:  on Linx using SCHED_OTHER, both values are 0
  minprio = sched_get_priority_min(schedpolicy);
  maxprio = sched_get_priority_max(schedpolicy);
  printf("priority in range [%d, %d]\n", minprio, maxprio);

  // an example of a voluntary yeild
  sched_yield();   // no blocking, put at end of runqueue

  return 0;
}
