#include <time.h>
#include <iostream>
#include <errno.h>
#include <stdio.h>

int main (int argc, char **argv)
{

  // Linux support a number of clocks in the kernel. The two most important
  // clocks are: CLOCK_REALTIME and CLOCK_MONOTONIC

  // CLOCK_REALTIME : tracks the time of day and is often adjusted to keep
  // the time synchronized with another server. Typically this is done via
  // internet time servers using a protocol called NTP. Due to this
  // synchronization it is possible that CLOCK_REALTIME is adjusted backwards


  // CLOCK_MONOTONIC: is a montonically increasing clock that counts the
  // number of seconds that have elapsed since the computer was turned on. as
  // the name suggests, this clock is never adjusted backwards.

  // to get the time, we need to use the struct timespec.

  struct timespec tmono, treal, tmonores, trealres;

  //let us get the clock resolution
  int retvalue = clock_getres (CLOCK_MONOTONIC, &tmonores);
  if (retvalue < 0)
    {
      perror ("monotonic clock_getres");
      return retvalue;
    }

  retvalue = clock_getres (CLOCK_REALTIME, &trealres);
  if (retvalue < 0)
    {
      perror ("realtime clock_getres");
      return retvalue;
    }
  // timespec has two fields tv_sec and tv_nsec.

  //Let us get the current montonic clock time.

  retvalue = clock_gettime (CLOCK_MONOTONIC, &tmono);
  if (retvalue < 0)
    {
      perror ("monotonic clock");
      return retvalue;
    }
  retvalue = clock_gettime (CLOCK_REALTIME, &treal);
  if (retvalue < 0)
    {
      perror ("realtime clock");
      return retvalue;
    }

  std::cout << "clock realtime resolution is " << trealres.tv_sec
            << " seconds and "
                "" << trealres.tv_nsec << std::endl;
  std::cout << "clock realtime time value is " << treal.tv_sec
            << " seconds and "
                "" << treal.tv_nsec << std::endl;

  std::cout << "clock montonic resolution is " << tmonores.tv_sec
            << " seconds and "
                "" << tmonores.tv_nsec << std::endl;

  std::cout << "clock montonic time value is " << tmono.tv_sec
            << " seconds and "
                "" << tmono.tv_nsec << std::endl;


  //Let us sleep by adding 10 seconds to clock realtime.
  timespec twait = {treal.tv_sec += 10, treal.tv_nsec};
  timespec tremain = {0, 0};
  std::cout << "clock realtime sleeping till value is " << twait.tv_sec
            << " seconds "
                "and "
                "" << twait.tv_nsec << std::endl;

  retvalue = clock_nanosleep (CLOCK_REALTIME, TIMER_ABSTIME, &twait, &tremain);
  while (retvalue != 0)
    {
      //sleep can be interrupted so keep sleeping until the revalue is 0
      // look at http://man7.org/linux/man-pages/man2/clock_nanosleep.2.html
      retvalue = clock_nanosleep (CLOCK_REALTIME, TIMER_ABSTIME, &twait, &tremain);

    }

  retvalue = clock_gettime (CLOCK_REALTIME, &treal);
  if (retvalue < 0)
    {
      perror ("realtime clock");
      return retvalue;
    }

  std::cout << "clock realtime time value after sleep is " << treal.tv_sec
            << " "
                "seconds and "
                "" << treal.tv_nsec << std::endl;
}
