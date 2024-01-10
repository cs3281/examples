#include <stdio.h>
#include <pthread.h>
//#include "mythreads.h"

static volatile int counter = 0;

//
// mythread()
//
// Simply adds 1 to counter repeatedly, in a loop
// No, this is not how you would add 10,000,000 to
// a counter, but it shows the problem nicely.
//

//adding a mutex.
pthread_mutex_t mutexsum;
 
void *
mythread(void *arg)
{
    printf("%s: begin\n", (char *) arg);
    int i;
	pthread_mutex_lock (&mutexsum);
    for (i = 0; i < 1e7; i++) {
        counter = counter + 1;
    }
	pthread_mutex_unlock (&mutexsum);
    printf("%s: done\n", (char *) arg);
    return NULL;
}

//
// main()
//
// Just launches two threads (pthread_create)
// and then waits for them (pthread_join)
//
int
main(int argc, char *argv[])
{
    pthread_t p1, p2;
	//initializing mutex.
	pthread_mutex_init(&mutexsum, NULL);
    printf("main: begin (counter = %d)\n", counter);
    pthread_create(&p1, NULL, mythread, "A");
    pthread_create(&p2, NULL, mythread, "B");

    // join waits for the threads to finish
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("main: done with both (counter = %d)\n", counter);
    return 0;
}
