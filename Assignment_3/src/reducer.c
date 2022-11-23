#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/Constants.h"

void reducer_process(int reducer_id, int shmids[], char *words[], int words_num,
                     int N) {
  char *sharedMsg;
  int word_counter = 0;
  // For each worker process
  for (int i = 0; i < N; i++) {
    sharedMsg = (char *)shmat(shmids[i], NULL, 0);
    while (strncmp(sharedMsg, TERMINATION_FLAG, strlen(TERMINATION_FLAG)) != 0)
      ;
    if (verbose) {
      printf("Message read by \x1B[%dmreducer %d\x1B[37m from \x1B[%dmworker "
             "%d\x1B[37m: %s\n",
             REDUCER_COLOR_ID, reducer_id, WORKER_COLOR_ID, i, sharedMsg);
    }

    int occurrences[words_num];
    char *ptr = sharedMsg + strlen(TERMINATION_FLAG) - 1;
    for (int j = 0; j < words_num; j++) {
      ptr = strstr(ptr + 1, " ");
      sscanf(ptr, " %d", &occurrences[j]);
    }
    shmdt(sharedMsg);
    word_counter += occurrences[reducer_id];
    // printf("\x1B[%dmReducer %d\x1B[37m read worker %d\n", REDUCER_COLOR_ID,
    // reducer_id, i);
  }
  printf("\x1B[%dmReducer %d\x1B[37m process terminated successfully.\n",
         REDUCER_COLOR_ID, reducer_id);
  printf("\t%s counter: %d\n", words[reducer_id], word_counter);
}
