#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
  int c1 = 0;
  c1 = fork ();
  if (c1 == 0)
    {

      /* we enter this block only if fork returns 0,
         which indicates that we are the child process */
      printf ("Value of var from child = %d\n", var);
      ++var;                    // var is only incremented in the child process
    }
  else
    {

      // the parent process does not increment var; it sleeps instead
      int status;
      int retvalue = 0;
      while (retvalue != c1)

        {
          status = 0;
          retvalue = waitpid (c1, &status, 0);
          if (retvalue < 0)

            {
              char buffer[256];
              strerror_r (errno, buffer, 256);
              printf ("error occured %s\n", buffer);
              break;
            }

          else

            {
              printf ("state of process %d changed - ", retvalue);
              if (WIFEXITED (status))
                {
                  printf ("exited, status=%d\n", WEXITSTATUS (status));
                }
              else if (WIFSIGNALED (status))
                {
                  printf ("killed by signal %d\n", WTERMSIG (status));
                }
              /* These conditions can never be true because we invoked waitpid with 0 option.
                 else if (WIFSTOPPED (status))
                 {
                 printf ("stopped by signal %d\n", WSTOPSIG (status));
                 }
                 else if (WIFCONTINUED (status))
                 {
                 printf ("continued\n");
                 } */
            }
        }
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
