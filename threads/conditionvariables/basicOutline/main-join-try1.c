#include <stdio.h>
#include <pthread.h>
#include "mythreads.h"

volatile int done = 0;

void *
child(void *arg) {
    printf("child\n");
    done = 1;
    return NULL;
}


int
main(int argc, char *argv[]) {
    pthread_t p;
    printf("parent: begin\n");
    Pthread_create(&p, NULL, child, NULL);
    while (done == 0) 
	; // spin (inefficient)
    printf("parent: end\n");
    return 0;
}
