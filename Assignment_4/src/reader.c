#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/Constants.h"

double reader_critical_section(Range range) {
  int value = 0;
  for (int i = range.first; i < range.last; i++) {
    value += array[i];
  }
  return (double)value / (range.last - range.first);
}

void read(Range range) {
  double mean;
  pthread_mutex_lock(&mutex);
  mean = reader_critical_section(range);
  pthread_mutex_unlock(&mutex);
  printf("Reader [%d-%d] mean %f\n", range.first, range.last, mean);
}

void *reader(void *args) {
  Range range = *(Range *)args;
  read(range);
  return NULL;
}