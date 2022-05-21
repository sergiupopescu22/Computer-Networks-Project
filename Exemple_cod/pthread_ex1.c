#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5

void *HelloWorld(void *threadid)
{
   long tid;
   tid = (long)threadid;
   printf("Hello World! Nr. fir executie: #%ld!\n", tid); 
   pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
   pthread_t thread[NUM_THREADS];
   int rc;
   long t;
   for(t=0; t<NUM_THREADS; t++)
   {
      printf("In main: creare fir %ld\n", t);
      rc = pthread_create(&thread[t], NULL, HelloWorld, (void *)t);
      if (rc)
      {
         printf("Codul erorii este: %d\n", rc);
         exit(-1);
      }
   }
   pthread_exit(NULL);
}
