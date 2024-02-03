#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void
handlechildexit(int num)
{
  printf("Received the signal SIGCHLD! Our child process terminated.\n");
}

int
main(int argc, char *argv[])
{
  struct sigaction sa;

  // initialize sa
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handlechildexit;

  // install sa to handle the SIGCHLD signal
  if (sigaction(SIGCHLD, &sa, 0) == -1) {
    printf("Error installing signal handler!\n");
    exit(-1);
  }

  // create a child process
  int pid = fork();

  if (pid == 0) {
    // let the child sleep for 2 seconds
    sleep(2);
  }
  
  // if we are the parent
  if (pid > 0) {
    int remaining = sleep(20); //sleep for 20 seconds? why does execution resume so soon?
    printf("Done sleeping! But we have %d time left.\n", remaining);
  }
}
