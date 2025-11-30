
#ifndef SMOKE_H
#define SMOKE_H

#include "uthread.h"
#include "uthread_mutex_cond.h"

#define NUM_ITERATIONS 1000

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__)
#else
#define VERBOSE_PRINT(S, ...) ((void) 0) // do nothing
#endif

struct Agent {
  uthread_mutex_t mutex;
  uthread_cond_t  match;
  uthread_cond_t  paper;
  uthread_cond_t  tobacco;
  uthread_cond_t  smoke;
  int num_waiting; // Count of threads to wait for until agent starts signalling
};

// One-hot encoding of resources
// ie. 0b001 is match, 0b010 is paper, 0b100 is tobacco
enum Resource            {    MATCH = 1, PAPER = 2,   TOBACCO = 4};

// Returns an array that holds the amount of times each smoker smokes
int* smoker_init(struct Agent* agent); // Initialization code

#endif