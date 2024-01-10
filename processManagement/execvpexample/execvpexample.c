#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int
main (int argc, char *argv[])
{
  pid_t c1, c2;

  char **args = malloc (5 * sizeof (char *));

  c1 = fork ();                 // Create a child process

  // Sort the first input file in the first child
  if (c1 == 0)
    {
      execl ("/usr/bin/sort", "sort", "-n", "input1.txt", "-o", "output1.txt",
             (char *) NULL);
      exit (0);                 // make sure we exit so the child stops here
    }

  c2 = fork ();                 // Create another child process

  // Sort the second input file in the second child
  if (c2 == 0)
    {
      execl ("/usr/bin/sort", "sort", "-n", "input2.txt", "-o", "output2.txt",
             (char *) NULL);
      exit (0);                 // exit the child here
    }

  // Wait for both children
  int status;
  waitpid (c1, &status, 0);
  waitpid (c2, &status, 0);

  // Construct the arguments for execvp
  // If you're using C++ and new, you may
  // have to use strcpy to get around const-ness issues
  args[0] = "sort";
  args[1] = "-m";
  args[2] = "output1.txt";
  args[3] = "output2.txt";
  args[4] = 0;

  // Invoke the final sort to merge the output files
  // This will write the result to standard output (the terminal)
  execvp ("sort", args);
}
