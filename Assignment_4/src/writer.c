#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/Constants.h"

void writer_critical_section(Range range) {
  for (int i = range.first; i < range.last; i++) {
    array[i] = rand();
  }
}

void write(Range range) {
  pthread_mutex_lock(&mutex);
  writer_critical_section(range);
  pthread_mutex_unlock(&mutex);
  printf("Writer [%d-%d]\n", range.first, range.last);
}

void *writer(void *args) {
  Range range = *(Range *)args;
  write(range);
  return NULL;
}