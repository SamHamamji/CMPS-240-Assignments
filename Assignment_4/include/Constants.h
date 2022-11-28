#pragma once

#include <semaphore.h>

#define length 10000000
#define nr 5  // number of readers
#define nw 5  // number of writers
#define n 100 // total number of executions

#define WRITER_COLOR_ID 31
#define READER_COLOR_ID 34

typedef enum thread_state { reading, finishing, writing } State;

typedef struct range {
  int first;
  int last;
} Range;

extern pthread_mutex_t m;
extern pthread_mutex_t wrt;
extern pthread_mutex_t order;
extern int read_counter;

extern pthread_mutex_t total_lock;
extern int total_executions;

extern int array[length];
extern State current_state;