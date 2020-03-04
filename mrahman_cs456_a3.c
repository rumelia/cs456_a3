// mrahman_cs456_a3.c
// CS456 Assignment 3 : Threads and Synchronization
// A multi-threaded program that manages synchronization in a
// shared-memory environment

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>

// declare global virus_count_array
// holds the # of viruses each friend let in through their door
// mapping: index n -> virus count for fn
int virus_count_array[4] = {0, 0, 0, 0};

// declare array of semaphores - one for each friend thread
// mapping: index n -> semaphore for fn
sem_t semaphore_array[4];

// declare variable n
// if viruses_in_building < n - OK, else close doors till viruses_in_building < n/2
int n;

// initialize ten millisecond time struct
struct timespec ten_milliseconds = {0, 10000000L};

void * neutralize_and_check(void *arg) {
  int loop_count = 0;
  int viruses_neutralized = 0;  // viruses neutralized since beginning of program
  int viruses_let_in = 0;       // viruses let in since beginning of program
  int viruses_in_building = 0;  // viruses_in_building since last check
  int doors_closed = 0;      // bool denoting whether or not doors are closed

  while(1) {
    nanosleep(&ten_milliseconds, NULL); // sleep for 10ms
    viruses_in_building = viruses_let_in - viruses_neutralized;

    if (viruses_in_building > 0) {
      // neutralize viruses every 10ms with a 40% probability
      float rand_num = (double)random() / RAND_MAX;
      if (rand_num <= 0.4f) {
        viruses_neutralized++;
      }
    }

    if (doors_closed && viruses_in_building < (n/2)) {
      printf("\n\nVIRUS COUNT UNDER CONTROL(=%d)\n", (n/2));
      printf("--------------------OPENING DOORS--------------------------\n\n");

      doors_closed = 0;

      // call post on all semaphores to wake corresponding friend
      // threads up in a controlled manner
      for (int i=0; i<4; i++) {
        sem_post(&semaphore_array[i]);
      }
    }

    // if loop_count == 200, approx 2 seconds have elapsed, so check the total number of viruses let in
    if (loop_count == 200) {
      // calculate total # of viruses_let_in through the doors
      viruses_let_in = 0;                 // reset viruses_let_in to 0
      viruses_in_building = 0;            // reset viruses_in_building to 0

      for (int i=0; i<4; i++) {
        viruses_let_in += virus_count_array[i];
      }

      viruses_in_building = viruses_let_in - viruses_neutralized;
      printf("\n--------------------STATS--------------------------\n");
      printf("Total no. of viruses: \n");
      printf("LET IN: %d\n", viruses_let_in);
      printf("NEUTRALIZED: %d\n", viruses_neutralized);
      printf("CURRENTLY IN BUILDING: %d\n\n", viruses_in_building);
      printf("--------------------STATS--------------------------\n");

      if (!doors_closed && viruses_in_building > n) {
        printf("\n\nWARNING: NUMBER OF VIRUSES IN BUILDING EXCEEDED MAX ALLOWED(=%d)\n", n);
        printf("--------------------CLOSING DOORS--------------------------\n\n");

        doors_closed = 1;

        // call wait on all semaphores to put corresponding friend
        // threads to sleep in a controlled manner
        for (int i=0; i<4; i++) {
          sem_wait(&semaphore_array[i]);
        }
      }
      loop_count = 0;   // reset loop_count to 0
    }
    loop_count++;
  }
  return NULL;
}

// function to let in viruses every 10ms with a 10% probability
void * let_in_viruses(void* arg) {
  unsigned long door_number = (unsigned long)arg;

  // semaphores for synchronization
  sem_wait(&semaphore_array[door_number]);
  sem_post(&semaphore_array[door_number]);

  while(1) {
    nanosleep(&ten_milliseconds, NULL);
    float rand_num = (float)random() / RAND_MAX;   // generate random value between 0 and 1
    if (rand_num <= 0.1f) {
      virus_count_array[door_number]++;
    }
  }
}


int main (int argc, char **argv) {

  printf("Please enter the value for n: ");
  scanf("%d", &n);
  printf("Maximum number of viruses allowed in the building: %d\n", n);

  // initialize all semaphores
  for (int i=0; i<4; i++) {
    printf("Initializing sem%d\n", i);
    sem_init(&semaphore_array[i], 0, 1);
  }

  // declare me and friend threads
  pthread_t me;
  pthread_t f0, f1, f2, f3;

  // assign me thread ----------------------------------
  if (pthread_create(&me, NULL, &neutralize_and_check, NULL)) {
    printf("Could not create ME thread\n");
    return -1;
  }

  // assign friend threads ------------------------------
  if (pthread_create(&f0, NULL, &let_in_viruses, (void*)0)) {
    printf("Could not create F0 thread\n");
    return -1;
  }

  if (pthread_create(&f1, NULL, &let_in_viruses, (void*)1)) {
    printf("Could not create F1 thread\n");
    return -1;
  }

  if (pthread_create(&f2, NULL, &let_in_viruses, (void*)2)) {
    printf("Could not create F2 thread\n");
    return -1;
  }

  if (pthread_create(&f3, NULL, &let_in_viruses, (void*)3)) {
    printf("Could not create F3 thread\n");
    return -1;
  }

  if (pthread_join(me, NULL)) {
    printf("Could not join ME thread\n");
    return -1;
  }

  return 0;
}
