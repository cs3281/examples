#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "mythreads.h"

pthread_cond_t  c = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int done = 0;

void *
child(void *arg) {
    printf("child\n");
    done = 1;
    Cond_signal(&c);
    return NULL;
}
int main(int argc, char *argv[]) {
    pthread_t p;
    printf("parent: begin\n");
    Pthread_create(&p, NULL, child, NULL);
    while (done == 0) 
	Cond_wait(&c, &m); 
    printf("parent: end\n");
    return 0;
}

