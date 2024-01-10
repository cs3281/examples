#include <stdio.h>
#include <pthread.h>
#include "mythreads.h"

int done = 0;

void *
child(void *arg) {
    printf("child\n");
    // put something here to signal "all done" ...
    return NULL;
}

int
main(int argc, char *argv[]) {
    pthread_t p;
    printf("parent: begin\n");
    Pthread_create(&p, NULL, child, NULL);
    // put something here to wait for the child
    printf("parent: end\n");
    return 0;
}

