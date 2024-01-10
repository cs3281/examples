#include <stdio.h>

/* This program demonstrates that not initializing variables
   can result in reusing values that were previously pushed
   onto the stack.
   On most machines, this will print 37 twice. In some cases,
   it may not.
   Lesson: always initialize variables before you use them. */

// For an explanation of the stack and automatic variables,
// see page 31 of the K & R book.

void
f1 ()
{
  // x is an automatic variable allocated on the stack
  int x = 37;

  printf ("Value of x is: %d\n", x);

  // after f1 exits, the variables it allocated on the stack
  // (such as x) can no longer be used.
  // however, the value 37 may still be in memory
}

void
f2 ()
{
  // x is an automatic variable but it isn't initialized with a value
  int x;

  // if the line below prints 37, it's because the variable x here
  // in f2 is using exactly the same address as the variable x in f1
  printf ("Value of x is: %d\n", x);
}

int
main (int argc, char *argv[])
{
  f1 ();
  f2 ();
  return 0;
}
