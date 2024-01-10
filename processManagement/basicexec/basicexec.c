#include <stdio.h>
#include <unistd.h>

int
main (int argc, char *argv[])
{
  int pid;

  pid = fork ();

  if (pid == 0)
    {
      execl ("/usr/bin/sort", "sort", "-n", "input.txt", (char *) NULL);
    }
  return 0;
}
