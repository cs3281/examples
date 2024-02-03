#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void
handlesegfault(int num)
{
  printf("Received the SIGSEGV signal!\n");
}

int
main(int argc, char *argv[])
{
  struct sigaction sa;

  // initialize sa
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handlesegfault;

  // install sa to handle the SIGSEGV signal
  if (sigaction(SIGSEGV, &sa, 0) == -1) {
    printf("Error installing signal handler!\n");
    exit(-1);
  }

  // do a normal array reference
  int arr[] = {1, 2, 3, 4, 5};
  printf("Value of arr at index 4 = %d\n", *(arr + 4));

  // uh oh! this line is an invalid memory reference
  int val = *(int *)0;

  // question: will the line below ever execute?
  printf("After handline\n");
}
