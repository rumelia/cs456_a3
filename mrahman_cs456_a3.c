// mrahman_cs456_a3.c
// CS456 Assignment 3 : Threads and Synchronization
// A multi-threaded program that manages synchronization in a
// shared-memory environment

#include <pthread.h>
#include <stdio.h>

// declare global virus_count_array
// holds the # of viruses each friend let in through their door
// mapping: index n -> virus count for fn
int virus_count_array[4];

void * neutralize_viruses(void *arg)
{

}

void * let_in_viruses(void  *arg)
{

}

int main (int argc, char **argv)
{
  // declare me and friend threads
  pthread_t me;
  pthread_t f0, f1, f2, f3;

  // assign me thread ----------------------------------
  if (pthread_create(&me, NULL, &neutralize_viruses, NULL))
  {
    printf("Could not created ME thread\n");
    return -1;
  }

  // assign friend threads ------------------------------
  if (pthread_create(&f0, NULL, &let_in_viruses, NULL))
  {
    printf("Could not created F0 thread\n");
    return -1;
  }

  if (pthread_create(&f1, NULL, &let_in_viruses, NULL))
  {
    printf("Could not created F1 thread\n");
    return -1;
  }

  if (pthread_create(&f2, NULL, &let_in_viruses, NULL))
  {
    printf("Could not created F2 thread\n");
    return -1;
  }

  if (pthread_create(&f3, NULL, &let_in_viruses, NULL))
  {
    printf("Could not created F3 thread\n");
    return -1;
  }

  return 0;
}
