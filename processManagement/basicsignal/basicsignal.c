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
  printf ("From process %d, received signal %d\n", getpid (), signum);
  if (signum == SIGTERM)
    {
      printf ("From process %d, exiting.\n", getpid ());
      exit (0);
    }
  if (signum == SIGCHLD)
    {
      printf ("From process %d, a child process has finished execution\n",
              getpid ());
      int status = 0;
      int child_pid = wait (&status);
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
          else if (WIFSTOPPED (status))
            {
              printf ("stopped by signal %d\n", WSTOPSIG (status));
            }
          else if (WIFCONTINUED (status))
            {
              printf ("continued\n");
            }
        }
    }

}

int
main (int argc, char *argv[])
{
  int var = 1;

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
      if (signal (SIGINT, signal_handler) == SIG_ERR)
        {
          printf
            ("From process %d, error while installing handler for SIGINT\n",
             getpid ());
        }
      if (signal (SIGUSR1, signal_handler) == SIG_ERR)
        {
          printf
            ("From process %d, error while installing handler for SIGUSR1\n",
             getpid ());
        }
      if (signal (SIGTERM, signal_handler) == SIG_ERR)
        {
          printf
            ("From process %d, error while installing handler for SIGTERM\n",
             getpid ());
        }
      if (signal (SIGINT, signal_handler) == SIG_ERR)
        {
          printf
            ("From process %d, error while installing handler for SIGINT\n",
             getpid ());
        }


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
      if (signal (SIGCHLD, signal_handler) == SIG_ERR)
        {
          printf ("process %d: error while installing handler for SIGINT\n",
                  getpid ());
        }
      kill (childpid, SIGINT);
      printf ("From process %d, sent SIGINT to %d\n", getpid (), childpid);
      sleep (1);
      kill (childpid, SIGUSR1);
      printf ("From process %d, sent SIGUSR1 to %d\n", getpid (), childpid);
      sleep (1);
      kill (childpid, SIGTERM);
      printf ("From process %d, sent SIGTERM to %d\n", getpid (), childpid);
      sleep (1);
    }

  return 0;

}
