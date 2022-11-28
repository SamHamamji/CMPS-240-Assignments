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
  // Entry section
  pthread_mutex_lock(&order);
  sem_wait(&reading);
  pthread_mutex_lock(&m);
  read_counter++;
  if (read_counter == 1) // if first reader
    pthread_mutex_lock(&wrt);
  pthread_mutex_unlock(&m);
  pthread_mutex_unlock(&order);

  pthread_mutex_lock(&total_lock);
  if (total_executions >= n) {
    sem_post(&reading);
    pthread_mutex_unlock(&wrt);
    pthread_mutex_unlock(&total_lock);
    pthread_exit(NULL);
  }
  total_executions++;
  pthread_mutex_unlock(&total_lock);

  // Critical section
  int num;
  num = sem_getvalue(&reading, &num);
  if (num > MAX_READERS_NUM) {
    printf("ERROR: MAX READERS NUM HAS BEEN EXCEEDED");
    exit(EXIT_FAILURE);
  }

  mean = reader_critical_section(range);
  if (verbose)
    printf("\x1B[%dmReader\x1B[37m [%d-%d] mean %f\n", READER_COLOR_ID,
           range.first, range.last, mean);

  // Exit section
  sem_post(&reading);
  pthread_mutex_lock(&m);
  read_counter--;
  if (read_counter == 0) // if last reader
    pthread_mutex_unlock(&wrt);
  pthread_mutex_unlock(&m);
}

void *reader(void *args) {
  Range range = *(Range *)args;
  while (true) {
    read(range);
  }
}