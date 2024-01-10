#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>

int
main (int argc, char *argv[])
{
  int var = 1;

  /* The fork system call is used by a process to request
     that the kernel create a new process.

     After fork has been called a new child process exists.
     Both the parent and the child then keep executing
     from the point right after the call to fork.
     Thus, both the parent and the child process both
     get a return value from fork.

     The child process gets a return value of 0, and
     the parent process gets a return value equal to the
     process ID of the newly created child.
   */

  if (fork () == 0)
    {
      /* we enter this block only if fork returns 0,
         which indicates that we are the child process */
      printf ("Value of var from child = %d\n", var);
      ++var;                    // var is only incremented in the child process
    }
  else
    {
      // the parent process does not increment var; it sleeps instead
      sleep (2);
    }

  /* The line below will be executed twice: once from the
     child and once from the parent.

     Recall that the parent process and child process have
     separate address spaces; thus, because only the child
     incremented the value of var, only the child will
     print that the value of var is 2.
   */

  printf ("From process %d, value of var is %d.\n", getpid (), var);
  return 0;
}
