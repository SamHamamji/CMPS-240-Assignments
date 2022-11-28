#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/Constants.h"
#include "../include/reader.h"
#include "../include/writer.h"

int main() {
  srand((unsigned int)time(NULL));
  if (pthread_mutex_init(&total_lock, NULL) != 0) {
    printf("Mutex total_lock init has failed\n");
    return 1;
  }
  if (pthread_mutex_init(&m, NULL) != 0) {
    printf("Mutex total_lock init has failed\n");
    return 1;
  }
  if (pthread_mutex_init(&wrt, NULL) != 0) {
    printf("Mutex total_lock init has failed\n");
    return 1;
  }
  if (pthread_mutex_init(&order, NULL) != 0) {
    printf("Mutex total_lock init has failed\n");
    return 1;
  }

  // initialize threads
  const int writer_sub_array_length = 1 + (int)((-1.0 + length) / (nw));
  pthread_t writers[nw];
  Range writers_range[nw];
  const int reader_sub_array_length = 1 + (int)((-1.0 + length) / (nr));
  pthread_t readers[nr];
  Range readers_range[nr];

  for (int i = 0; i < nw; i++) {
    writers_range[i].first = i * writer_sub_array_length;
    if (i == nw - 1)
      writers_range[i].last = length;
    else
      writers_range[i].last = (i + 1) * writer_sub_array_length;
  }

  for (int i = 0; i < nr; i++) {
    readers_range[i].first = i * reader_sub_array_length;
    if (i == nr - 1)
      readers_range[i].last = length;
    else
      readers_range[i].last = (i + 1) * reader_sub_array_length;
  }

  // run writers
  for (int i = 0; i < nw; i++) {
    pthread_create(&writers[i], NULL, writer, (void *)&writers_range[i]);
  }

  // run readers
  for (int i = 0; i < nr; i++) {
    pthread_create(&readers[i], NULL, reader, (void *)&readers_range[i]);
  }

  // join threads
  for (int i = 0; i < nw; i++) {
    pthread_join(writers[i], NULL);
  }

  for (int i = 0; i < nr; i++) {
    pthread_join(readers[i], NULL);
  }

  printf("\ntotal_executions: %d\n", total_executions);
  return 0;
}
