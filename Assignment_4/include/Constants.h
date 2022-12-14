#pragma once

#include <semaphore.h>

#define length 10000000   // array length
#define nr 5              // number of readers
#define nw 5              // number of writers
#define n 1000            // total number of executions
#define MAX_READERS_NUM 3 // max number of readers that can run simultaneously

#define WRITER_COLOR_ID 31
#define READER_COLOR_ID 34
#define verbose false

typedef struct range {
  int first;
  int last;
} Range;

extern pthread_mutex_t m;
extern pthread_mutex_t wrt;
extern pthread_mutex_t order;
extern sem_t reading;
extern int read_counter;

extern pthread_mutex_t total_lock;
extern int total_reading_executions;
extern int total_writing_executions;

extern int array[length];
