#include <stdlib.h>
#include <stdio.h>

#include "uthread.h"
#include "uthread_mutex_cond.h"
#include "smoke.h"

int smoke_count [5];  // # of times resource smoked
char* resource_name [] = {"", "match",   "paper", "", "tobacco"}; // May be useful for logging purposes

//
// TODO
// You will probably need to add some procedures and struct etc.
//

// Initialize any threads, data structures, etc. here
int* smoker_init(struct Agent* agent) {
  // TODO
  return smoke_count;
}
