#include <iostream>
#include <unistd.h>
int
main ()
{


  int pid = fork ();
  int mypid = getpid ();
  std::cout << "mypid is " << mypid << std::endl;
  return 0;
}
