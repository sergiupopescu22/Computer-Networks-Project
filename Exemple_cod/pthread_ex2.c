#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5
#define COUNT 5

int c=0;

void *count(void *threadid)
{
   int i;
   long tid;
   tid = (long)threadid;
   for (i=0;i<COUNT;i++)
   {
        c++;
        printf("Counter: %d, Nr. thread  %ld\n", c,tid);
        sleep(1);
   }
   pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
   int rc;
   long t;
   pthread_t thread[NUM_THREADS];
   for(t=0; t<NUM_THREADS; t++)
   {
      printf("In main: creare fir %ld\n", t);
      rc = pthread_create(&thread[t], NULL, count,(void *)t);
      if (rc)
      {
         printf("Codul erorii este: %d\n", rc);
         exit(-1);
      }
   }
   pthread_exit(NULL);
}
