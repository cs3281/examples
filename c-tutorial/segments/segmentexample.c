#include <stdio.h>

/* This program shows how segments work. */   

/* The variable x below is external to any function.
   Because it is external to any function, all functions
   in this file can access it.
   Because it is not initialized, its storage is allocated
   in the bss segment. */

int x;

/* The variable y below is also external to any function.
   Because of this, all functions in the file can access it.
   Because it is initialized, it is stored in the data
   segment. Because the program has to store the initial
   value, the size of the executable is larger (by the
   size of an integer) than it would be if we did not
   initialize y. */

int y = 1;

void foo()
{
  x = 2;
  printf("Inside foo, x = %d and y = %d\n", x, y);
}

/* The function bar() below defines a static variable
   named z. Because z is defined inside the function
   bar, it is only visible inside the bar() function.
   In other words, it is local to the bar() function.
   However, because it is defined as static, it 
   remains in existence rather than coming and going
   each time the function bar() is called.
   Thus, internal static variables provide private,
   permanent storage within a single function.
   Again, because the program has to store the initial
   value of z, the size of the executable is larger
   (by the size of an integer) than it would be
   if we did not initialize z. */

void bar()
{
  static int z = 3; /* initialize z to 3 initially */
  printf("Inside bar, z = %d\n", z);
  
  /* Because z remains in existence between invocations
     of bar(), when we increment z below, the
     next time bar is called it will print out
     that the value of z is 4. */

  z++;
}

/* In the function baz() below, the extern declaration
   can be omitted because the external variables
   we want to use, x and y, are defined above
   this function in the same file.
   In other words, we can omit the line:
   extern int x, y;
   entirely from the baz function and still print
   the values of x and y.
 */

void baz()
{
  extern int x, y;
  printf("Inside baz, x = %d and y = %d\n", x, y);
}

int main(int arg, char *argv[])
{
  foo();
  bar();
  bar();
  baz();
}
