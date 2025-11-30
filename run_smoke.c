#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "uthread.h"
#include "uthread_mutex_cond.h"
#include "smoke.h"

int signal_count [5];  // # of times resource signalled

struct Agent* createAgent() {
  struct Agent* agent = malloc (sizeof (struct Agent));
  agent->mutex   = uthread_mutex_create();
  agent->paper   = uthread_cond_create(agent->mutex);
  agent->match   = uthread_cond_create(agent->mutex);
  agent->tobacco = uthread_cond_create(agent->mutex);
  agent->smoke   = uthread_cond_create(agent->mutex);
  agent->num_waiting = 3; // Starter code, you can overwrite this in smoker_init if necessary
  return agent;
}

/**
 * This is the agent procedure. 
 * It chooses 2 random resources, signals their condition variables, and then waits
 */
void* agent (void* av) {
  struct Agent* a = av;
  static const int choices[]         = {MATCH|PAPER, MATCH|TOBACCO, PAPER|TOBACCO};
  static const int matching_smoker[] = {TOBACCO,     PAPER,         MATCH};
  srandom(time(NULL));
  
  uthread_mutex_lock (a->mutex);

  // Wait until all other threads are waiting for a signal
  while (a->num_waiting > 0) {
    uthread_cond_wait (a->smoke);
  }
  
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    int r = random() % 3;
    signal_count [matching_smoker[r]] ++;
    int c = choices [r];
    if (c & MATCH) {
      VERBOSE_PRINT ("match available\n");
      uthread_cond_signal (a->match);
    }
    if (c & PAPER) {
      VERBOSE_PRINT ("paper available\n");
      uthread_cond_signal (a->paper);
    }
    if (c & TOBACCO) {
      VERBOSE_PRINT ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
    }
    VERBOSE_PRINT ("agent is waiting\n");
    uthread_cond_wait (a->smoke);
  }
  
  uthread_mutex_unlock (a->mutex);
  return NULL;
}

int main() {
  uthread_init(5);

  struct Agent* a = createAgent();
  uthread_t agent_thread = uthread_create(agent, a);

  int* smoke_count = smoker_init(a);
  uthread_join(agent_thread, NULL);

  assert (signal_count [MATCH]   == smoke_count [MATCH]);
  assert (signal_count [PAPER]   == smoke_count [PAPER]);
  assert (signal_count [TOBACCO] == smoke_count [TOBACCO]);
  assert (smoke_count [MATCH] + smoke_count [PAPER] + smoke_count [TOBACCO] == NUM_ITERATIONS);
  printf ("Smoke counts: %d matches, %d paper, %d tobacco\n",
          smoke_count [MATCH], smoke_count [PAPER], smoke_count [TOBACCO]);
}