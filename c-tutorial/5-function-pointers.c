#include <stdio.h>
#include <stdlib.h>

/* This program demonstrates how to use function pointers.
   For an explanation of function pointers, see page 118
   in the K & R book. */

int *
f1 ()
{
  printf ("Inside f1\n");

  // dynamically allocate a single int on the heap
  // for an explanation of the heap and dynamic memory allocation,
  // see page 167 in the K & R book.
  int *x = (int *) malloc (sizeof (int));

  // it is safe to return this address because it is somewhere in
  // the heap. it would not be safe to return the address of an
  // automatic variable that was allocated on the stack
  return x;
}

int *
f2 ()
{
  printf ("Inside f2\n");
  int *x = (int *) malloc (sizeof (int));
  return x;
}

void
f3 (int x)
{
  printf ("Inside f3!\n");
}

int
main (int argc, char *argv[])
{
  // use the right to left rule to decipher this declaration:
  // g is a pointer to a function that takes an int and returns void.
  void (*g3) (int);

  // f3 is a function that takes an int and returns void, so we can
  // assign its address to g3
  g3 = f3;

  // we can invoke the function that g3 points to like this
  (*g3) (3);

  // let's declare an array of function pointers to hold the addresses
  // of f1 and f2.
  int *(*arr[2]) ();

  // store the address of f1 in arr[0] and f2 in arr[1]
  arr[0] = f1;
  arr[1] = f2;

  // invoke the functions pointed to by arr[0] and arr[1]
  (*arr[0]) ();
  (*arr[1]) ();
  return 0;
}
