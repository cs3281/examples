#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* This variable will be incremented without protection */
int varWithoutMutex = 0;

/* This variable will be incremented with protection by
   a mutex
*/
int varWithMutex = 0;

/* Mutexes must always be initialized before use.
   You can initialize a statically allocated mutex
   by assigning it the value PTHREAD_MUTEX_INITIALIZER
*/
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* This is the function that will be called by our newly created threads */
void *
threadFunction(void *arg)
{
  int i, status;
  i = status = 0;

  printf("The value of varWithoutMutex in thread is %d\n", varWithoutMutex);

  /* Increment varWithoutMutex without using a mutex */
  for (i = 0; i < 10000; i++)
    varWithoutMutex++;

  for (i = 0; i < 10000; i++) {
    /* lock the mutex using pthread_mutex_lock */
    status = pthread_mutex_lock(&mutex);
    if (status != 0)
      pthread_exit(NULL);

    /* Increment varWithMutex using a mutex */
    varWithMutex++;

    /* unlock the mutex using pthread_mutex_unlock */
    status = pthread_mutex_unlock(&mutex);
    if (status != 0)
      pthread_exit(NULL);
  }
}

#define NUM_THREADS 10

int
main(int argc, char *argv[])
{
  int i, status;
  void *res;
  pthread_t *threads[10];

  /* allocate space for NUM_THREADS threads */
  for (i = 0; i < NUM_THREADS; i++) {
    threads[i] = (pthread_t*)malloc(sizeof(pthread_t));
  }

  /* create NUM_THREADS new threads */
  for (i = 0; i < NUM_THREADS; i++) {
    status = pthread_create(threads[i], NULL, threadFunction, NULL);
    if (status != 0)
      exit(1);
  }

  /* wait for the threads to terminate */
  for (i = 0; i < NUM_THREADS; i++) {
    status = pthread_join(*threads[i], &res);
    if (status != 0)
      exit(1);
  }

  /* clean up the memory */
  for (i = 0; i < NUM_THREADS; i++) {
    free(threads[i]);
  }

  /* Check the value of both global variables. They should both be 
     NUM_THREADS * 10000, which is 10 * 10000 = 100000. Run this
     program several times and see if this is always the case. */
  printf("The value of varWithoutMutex in main thread is %d\n", varWithoutMutex);
  printf("The value of varWithMutex in main thread is %d\n", varWithMutex);
}
