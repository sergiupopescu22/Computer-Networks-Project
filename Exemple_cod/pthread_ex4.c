#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5
#define COUNT 5

int c=0;
pthread_mutex_t mutex;

void *count(void *threadid)
{
   int i;
   long tid;
   tid = (long)threadid;
   pthread_mutex_lock (&mutex);
   for (i=0;i<COUNT;i++)
   {
        c++;
        printf("Counter: %d, Nr. thread  %ld\n", c,tid);
        sleep(1);
   }
   pthread_mutex_unlock (&mutex);
   pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
   pthread_t thread[NUM_THREADS];
   int rc;
   long t;
   void *status;
   pthread_mutex_init(&mutex, NULL);
   for(t=0; t<NUM_THREADS; t++)
   {
      printf("In main: creare fir:  %ld\n", t);
      rc = pthread_create(&thread[t], NULL, count,(void *)t);
      if (rc)
      {
         printf("Codul erorii este:  %d\n", rc);
         exit(-1);
      }
   }
   pthread_mutex_destroy(&mutex);
   pthread_exit(NULL);
}
