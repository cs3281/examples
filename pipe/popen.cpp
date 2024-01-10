#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

int main (int argc, char **argv)
{
  int fds[2];

  if (pipe (fds) != 0)
    {
      perror ("pipe:");
      return -1;
    }

  std::cerr << "Forking...\n";

  pid_t pid = fork ();

  if (pid > 0)
    for (size_t i = 0; i < 10; ++i)
      {
      //always close the end you are not using
        close(fds[0]);
        char buf[5];
        int count = snprintf (buf, 5, "%lu ", i);
        write (fds[1], buf, count);
        sleep (1);
      }
  else if (pid == 0)
    {
      //always close the end you are not using
        close(fds[1]);
      for (;;)
        {
          char buf;
          // Typically, you'd want to read more than a single byte at a time...
          ssize_t bytes = read (fds[0], &buf, 1);
          if (bytes < 1) break;
          std::cerr << buf;
        }
      std::cerr << "Parent closed the pipe, exiting!\n";
      exit (0);
    }
  else
    {
      perror ("fork");
      return -1;
    }

  // in parent
  std::cerr << "Closing the write side of the pipe...\n";

  close (fds[1]);
  int status;
  pid = wait (&status);

  return 0;
}
