#include <stdio.h>

/* This program demonstrates the use of pointer arithmetic.
   Pointers and arrays are very closely related.
   For an explanation, see page 97 in the K & R book. */

void
f1 (int *x)
{
  // (x + 1) refers to the second element in the array

  // the line below stores the value 31 in the second cell of the array
  *(x + 1) = 31;
}

int
main (int argc, char *argv[])
{
  // x is an array that will hold 3 ints.
  int x[3];

  // the first index contains the value 1
  x[0] = 1;

  // we can pass the address like this
  f1 (x);

  // we can also pass the address of the array like this
  f1 (&x[0]);

  printf ("Value of x[1] after calling function is: %d\n", x[1]);
  return 0;
}
