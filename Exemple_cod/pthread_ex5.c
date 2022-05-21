#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS     5
#define COUNT           7
#define REST            0

pthread_mutex_t mutex;
pthread_cond_t count_cond;

void *count(void *threadid)
{
   int i;
   int c=0;
   long tid;
   tid = (long)threadid;
   for (i=0;i<COUNT;i++)
   {
      pthread_mutex_lock (&mutex);
      c++;
      printf("Counter: %d, Nr. thread  %ld\n", c,tid);
      if ((c%5) == REST)
      {
        pthread_cond_signal(&count_cond);
      }
      pthread_mutex_unlock (&mutex);
      sleep(1);
   }
   pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
   pthread_t thread[NUM_THREADS];
   int rc;
   long t;
   void *status;
   pthread_mutex_init(&mutex, NULL);
   pthread_cond_init (&count_cond, NULL);
   pthread_mutex_lock (&mutex);
   for(t=0; t<NUM_THREADS; t++)
   {
      printf("In main: creare fir:  %ld\n", t);
      rc = pthread_create(&thread[t], NULL, count,(void *)t);
      if (rc)
      {
         printf("Codul erorii este:  %d\n", rc);
         exit(-1);
      }
      pthread_cond_wait(&count_cond,&mutex);
   }
   pthread_mutex_unlock (&mutex);
   sleep(50);
   pthread_mutex_destroy(&mutex);
   pthread_cond_destroy(&count_cond);
   pthread_exit(NULL);
}
