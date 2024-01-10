#define _GNU_SOURCE

#include <stdio.h>
#include "mythreads.h"
#include <stdlib.h>
#include <pthread.h>

int max;
volatile int balance = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *
mythread(void *arg)
{       
    char *letter = arg;
    cpubind();
    printf("%s: begin\n", letter);
    int i;
    Pthread_mutex_lock(&lock); 
    for (i = 0; i < max; i++) {
	balance++;
    }
    Pthread_mutex_unlock(&lock); 
    printf("%s: done\n", letter);
    return NULL;
}
                                                                             
int
main(int argc, char *argv[])
{                    
    if (argc != 2) {
	fprintf(stderr, "usage: main-first <loopcount>\n");
	exit(1);
    }
    max = atoi(argv[1]);

    pthread_t p1, p2;
    printf("main: begin [balance = %d]\n", balance);
    Pthread_create(&p1, NULL, mythread, "A"); 
    Pthread_create(&p2, NULL, mythread, "B");
    // join waits for the threads to finish
    Pthread_join(p1, NULL); 
    Pthread_join(p2, NULL); 
    printf("main: done\n [balance: %d]\n [should: %d]\n", 
	   balance, max*2);
    return 0;
}

