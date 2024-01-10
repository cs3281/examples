#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdlib.h>             /* exit, EXIT_FAILURE */
#include <stdio.h>

int main ()
{
    //The first part of the example shows the addresses for malloced locations
    int *i,*j=0;
    int counter=0;
    void * intialbrk=sbrk(0);
    printf("the current location of brk is %p\n",intialbrk);


    i = (int*) malloc (5*sizeof(int));
    j = (int*) malloc (5*sizeof(int));
    void * currentbrk=sbrk(0);
    printf("the location of brk after malloc is %p\n",currentbrk);

    //Question: Why is this different?
    printf("real allocated bytes = %d. We asked for %d\n",currentbrk-intialbrk,10*sizeof(int));


    //let us print out the pointer address of i
    for (counter=0;counter<5;counter++)
    {
        //we will store 1 in there.
        i[counter]=1;
        printf("address of i[%d]:%p: value stored is %d\n", counter, i+counter,i[counter]);


    }
    //let us print out the pointer address of j
    printf("there is no guarantee that j will begin immediately after i.\n");
    for (counter=0;counter<5;counter++)
    {
        //we store 2 in there
        j[counter]=2;
        printf("address of j[%d]:%p: value stored is %d\n",counter, j+counter,j[counter]);

    }

    printf("Now we check the value of i.\n");
    // Now let us try to access i to the 10 counter value. Why is it not failing?
    //let us print out the pointer address of i
    for (counter=0;counter<10;counter++) {
        printf("address of i[%d]:%p: value stored is %d\n", counter, i + counter, i[counter]);
    }

        return 0;

}