#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
void
signal_handler (int signum)
{

  if (signum == SIGCHLD)
    {
      printf ("From process %d, a child process has finished execution\n",
              getpid ());
      int status = 0;
      // Wait for any child process.
      int child_pid = 1;
      while (child_pid > 0)
        {
          child_pid = waitpid (-1, &status, WNOHANG);
          if (child_pid < 0)

            {
              char buffer[256];
              strerror_r (errno, buffer, 256);
              printf ("error occured %s\n", buffer);
              return;
            }

          else

            {
              printf ("state of process %d changed - ", child_pid);
              if (WIFEXITED (status))
                {
                  printf ("exited, status=%d\n", WEXITSTATUS (status));
                }
              else if (WIFSIGNALED (status))
                {
                  printf ("killed by signal %d\n", WTERMSIG (status));
                }
            }
        }
    }
}

int
main (int argc, char *argv[])
{
  int var = 1;
  if (signal (SIGCHLD, signal_handler) == SIG_ERR)
    {
      printf ("process %d: error while installing handler for SIGINT\n",
              getpid ());
    }
  /* The fork system call is used by a process to request
     that the kernel create a new process.:

     After fork has been called a new child process exists.
     Both the parent and the child then keep executing
     from the point right after the call to fork.
     Thus, both the parent and the child process both
     get a return value from fork.

     The child process gets a return value of 0, and
     the parent process gets a return value equal to the
     process ID of the newly created child.
   */

  int childpid = fork ();
  if (childpid == 0)
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
  if (childpid == 0)
    {
      while (1)
        sleep (1);
    }
  else
    {

      kill (childpid, SIGINT);
      printf ("From process %d, sent SIGINT to %d\n", getpid (), childpid);
      sleep (5);
    }
}
