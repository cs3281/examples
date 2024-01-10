#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
void
writehere (const char *msg)
{
  write (STDOUT_FILENO, msg, strlen (msg));
}

// async-safe implementation
void
positive_integer_to_string (int number, char *buffer, int length)
{
  // count number of digits
  int numdigits, num;
  if (number < 0)
    {
      number = -number;         // just in case
    }
  numdigits = 0;
  num = number;
  if (num)
    {
      do
        {
          numdigits++;
          num /= 10;
        }
      while (num);
    }
  else
    {
      numdigits = 1;
    }
  if (length < (numdigits + 1))
    return;
  int remainder, i;

  for (i = 0; i < numdigits; i++)
    {
      remainder = number % 10;
      number = number / 10;
      buffer[numdigits - (i + 1)] = remainder + '0';
    }
  buffer[numdigits] = '\0';
}

void
signal_handler (int signum)
{

  if (signum == SIGCHLD)
    {
      int status = 0;
      // Wait for any child process.
      int child_pid = 1;
      for (;;)
        {
          int child_pid = waitpid (-1, &status, WNOHANG);
          if (!child_pid)
            {
              break;
            }
          if (child_pid < 0)

            {
              char buffer[256];
              memset(buffer,0,256);
              strerror_r (errno, buffer, 256);
              write (1, buffer, strlen (buffer));
              write (1, "\n", 1);
              return;
            }

          else

            {

              if (WIFEXITED (status))
                {
                  writehere ("exited status ");
                  int exitcode = WEXITSTATUS (status);
                  char exitstring[20];
                  exitstring[0] = '\0';
                  positive_integer_to_string (exitcode, exitstring, 20);
                  writehere (exitstring);
                  writehere ("\n");
                }
              else if (WIFSIGNALED (status))
                {
                  writehere ("terminated by signal ");
                  int termsig = WTERMSIG (status);
                  char termsigstring[20];
                  termsigstring[0] = '\0';
                  positive_integer_to_string (termsig, termsigstring, 20);
                  writehere (termsigstring);
                  writehere ("\n");
                }
            }
        }
    }
}

int
main (int argc, char *argv[])
{
  int var = 1;

  struct sigaction new_action;
  new_action.sa_handler = signal_handler;
  sigemptyset (&new_action.sa_mask);
  new_action.sa_flags = SA_RESTART;
  // new_action.sa_flags=0;

  if (sigaction (SIGCHLD, &new_action, 0) == -1)
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
      // while (1)
      sleep (2);
    }
  else
    {

      // kill (childpid, SIGINT);
      printf ("From process %d, sent SIGINT to %d\n", getpid (), childpid);
      // sleep (5);
      std::string temp;
      while (true)
        {
          getline (std::cin, temp);
          std::cout << "read " << temp << " \n";
        }
    }
}
