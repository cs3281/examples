#define _GNU_SOURCE
#include <stdio.h>
#include "mythreads.h"
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <syscall.h>
int max;
volatile int balance = 0; // shared global variable

void *
mythread(void *arg)
{
    char *letter = arg;
    int i; // stack (private per thread) 
    printf("%s: begin [addr of i: %p]\n", letter, &i);
    for (i = 0; i < max; i++) {
	balance = balance + 1; // shared: only one
    }
    printf("%s: done\n", letter);

    printf("From the thread getpid: %d getpthread_self: %lu tid:%lu\n",getpid(), pthread_self(), syscall(SYS_gettid));
    return NULL;
}
                                                                             
int
main(int argc, char *argv[])
{                    
    printf("From the main process thread: getpid: %d getpthread_self: %lu tid:%lu\n",getpid(), pthread_self(), syscall(SYS_gettid));
    if (argc != 2) {
	fprintf(stderr, "usage: main-first <loopcount>\n");
	exit(1);
    }
    max = atoi(argv[1]);

    pthread_t p1, p2;
    printf("main: begin [balance = %d] [%x]\n", balance, 
	   (unsigned int) &balance);
    Pthread_create(&p1, NULL, mythread, "A"); 
    Pthread_create(&p2, NULL, mythread, "B");

    printf("From main thread, created threads, A:%lu and B:%lu\n",p1,p2);

    // join waits for the threads to finish
    Pthread_join(p1, NULL); 
    Pthread_join(p2, NULL); 
    printf("main: done\n [balance: %d]\n [should: %d]\n", 
	   balance, max*2);
    return 0;
}

