#pragma once

#include <semaphore.h>

#define length 10000
#define nr 9  // number of readersnw
#define nw 3  // number of writerenradriters
#define n 100 // total number of executions

typedef enum thread_state { reading, finishing, writing } State;

typedef struct range {
  int first;
  int last;
} Range;

extern pthread_mutex_t total_lock;
extern pthread_mutex_t mutex;

extern int array[length];
extern State current_state;