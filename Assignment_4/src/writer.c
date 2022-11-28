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
  // Entry section
  pthread_mutex_lock(&order);
  pthread_mutex_lock(&wrt);
  pthread_mutex_unlock(&order);

  pthread_mutex_lock(&total_lock);
  if (total_executions >= n) {
    pthread_mutex_unlock(&wrt);
    pthread_mutex_unlock(&total_lock);
    pthread_exit(NULL);
  }
  pthread_mutex_unlock(&total_lock);

  pthread_mutex_lock(&total_lock);
  total_executions++;
  pthread_mutex_unlock(&total_lock);

  // Critical section
  writer_critical_section(range);
  printf("\x1B[%dmWriter\x1B[37m [%d-%d]\n", WRITER_COLOR_ID, range.first,
         range.last);

  // Exit section
  pthread_mutex_unlock(&wrt);

  // Remainder section
  // printf("Writer [%d-%d]\n", range.first, range.last);
}

void *writer(void *args) {
  Range range = *(Range *)args;
  while (true) {
    write(range);
  }
}