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
    Mutex_lock(&m);
    done = 1;
    Cond_signal(&c);
    Mutex_unlock(&m);
    return NULL;
}
int main(int argc, char *argv[]) {
    pthread_t p;
    printf("parent: begin\n");
    Pthread_create(&p, NULL, child, NULL);
    Mutex_lock(&m);
    while (done == 0) 
	Cond_wait(&c, &m); // releases lock when going to sleep
    Mutex_unlock(&m);
    printf("parent: end\n");
    return 0;
}

