This folder shows the use of timer apis

1. clock_gettime: this api is used to obtain the current time of day from the kernel. The time of day is represented as seconds and nanoseconds since epoch, January 1 1970. This time is represented in a structure called timespec.

For more details, please see https://linux.die.net/man/3/clock_gettime

2. clock_nanosleep: this api is used to do a high resolution  sleep. See
http://man7.org/linux/man-pages/man2/clock_nanosleep.2.html for more information

