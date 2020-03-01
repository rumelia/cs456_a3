// mrahman_cs456_a3.c
// CS456 Assignment 3 : Threads and Synchronization
// A multi-threaded program that manages synchronization in a
// shared-memory environment

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// declare global virus_count_array
// holds the # of viruses each friend let in through their door
// mapping: index n -> virus count for fn
int virus_count_array[4] = {0, 0, 0, 0};
int viruses_let_in = 0;
int viruses_neutralized = 0;

// initialize two second and ten millisecond time structs
struct timespec two_seconds = {2, 0L};
struct timespec ten_milliseconds = {0, 10000000L};

// change this function, make it more modular use this just to check virus count
// create separate function for barrier implementor and use this function in that
// VIRUS SUM NEEDS TO BE GLOVAL so you can neutralize_viruses
void * check_virus_count(void *arg) {
  while(1) {
    nanosleep(&two_seconds, NULL);      // sleep for two seconds
    viruses_let_in = 0;                 // reset viruses_let_in to 0

    for (int i=0; i < 4; i++) {
      viruses_let_in += virus_count_array[i];
      printf("Door %d count: %d\n", i, virus_count_array[i]);
    }
    printf("Number of viruses neutralized: %d\n", viruses_neutralized);
    int viruses_in_building = viruses_let_in - viruses_neutralized;
    printf("Total number of viruses in the building: %d\n", viruses_in_building);
  }
  return NULL;
}

void * neutralize_viruses(void *arg) {
  while(1) {
    nanosleep(&ten_milliseconds, NULL);
    double rand_num = (double)rand() / RAND_MAX;
    if (rand_num <= 0.4) {
      viruses_neutralized++;
    }
  }
  return NULL;
}

// function to let in viruses every 10ms with a 10% probability-----------------
void let_in_viruses(int door_number) {
  while(1) {
    nanosleep(&ten_milliseconds, NULL);
    double rand_num = (double)rand() / RAND_MAX;   // generate random value between 0 and 1
    if (rand_num <= 0.1) {
      // printf("Letting in a virus through door %d!\n", door_number);
      printf("DOOR THREAD: Random number generated for DOOR %d: %lf\n", door_number, rand_num);
      virus_count_array[door_number]++;
    }
  }
}

// functions for friend threads; denoted door number corresponds to friend number
void * door0(void  *arg) {
  let_in_viruses(0);
  return NULL;
}

void * door1(void  *arg) {
  let_in_viruses(1);
  return NULL;
}

void * door2(void  *arg) {
  let_in_viruses(2);
  return NULL;
}

void * door3(void  *arg) {
  let_in_viruses(3);
  return NULL;
}


int main (int argc, char **argv) {
  // declare me and friend threads
  pthread_t me;
  pthread_t f0, f1, f2, f3;
  pthread_t virus_count_checker;

  // assign me thread ----------------------------------
  if (pthread_create(&me, NULL, &neutralize_viruses, NULL)) {
    printf("Could not create ME thread\n");
    return -1;
  }

  // assign friend threads ------------------------------
  if (pthread_create(&f0, NULL, &door0, NULL)) {
    printf("Could not create F0 thread\n");
    return -1;
  }

  if (pthread_create(&f1, NULL, &door1, NULL)) {
    printf("Could not create F1 thread\n");
    return -1;
  }

  if (pthread_create(&f2, NULL, &door2, NULL)) {
    printf("Could not create F2 thread\n");
    return -1;
  }

  if (pthread_create(&f3, NULL, &door3, NULL)) {
    printf("Could not create F3 thread\n");
    return -1;
  }

  if (pthread_create(&virus_count_checker, NULL, &check_virus_count, NULL)) {
    printf("Could not create CHECKER thread\n");
    return -1;
  }

  if (pthread_join(virus_count_checker, NULL)) {
    printf("Could not join CHECKER thread\n");
    return -1;
  }

  return 0;
}
