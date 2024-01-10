#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

void pexit(void){

printf("process dead\n");
}

void
cleanup(void *arg)
{
    printf("%ld: cleanup: %s\n", pthread_self(), (char *)arg);
}


void *mythread(void *arg) {
    printf("%s\n", (char *) arg);
 
    pthread_cleanup_push(cleanup, "thread first handler");
    pthread_cleanup_push(cleanup, "thread second handler");
    printf("thread  push complete\n");
    sleep(2);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    return NULL;
}

int
main(int argc, char *argv[]) {
    pthread_t p1, p2;
    int rc;
    atexit(pexit);
    printf("main: begin\n");
    rc = pthread_create(&p1, NULL, mythread, "A"); assert(rc == 0);
    rc = pthread_create(&p2, NULL, mythread, "B"); assert(rc == 0);
    // join waits for the threads to finish
    sleep(1);
    pthread_cancel(p2);

    rc = pthread_join(p1, NULL); assert(rc == 0);

    rc = pthread_join(p2, NULL); assert(rc == 0);
    printf("main: end\n");
    return 0;
}
