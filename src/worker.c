#include <stdio.h>   // used for printf()
#include <string.h>  // used for strstr()
#include <sys/shm.h> // used for shmget(), shmat(), ...

#include "../include/Constants.h"
/**
 Counts the occurrences of `string` in `file`
*/
int count_occurrences(char *file_path, char *word) {
  int string_length = 0;
  while (word[string_length] != '\0') {
    string_length++;
  }
  int counter = 0;
  FILE *file = fopen(file_path, "r");
  if (file == NULL) {
    return 0;
  }

  char buf[BUFFER_SIZE];
  char *ptr;
  while (fgets(buf, BUFFER_SIZE, file) != NULL) {
    ptr = buf;
    while ((ptr = strstr(ptr, word)) != NULL) {
      ptr++;
      counter++;
    }
  }
  fclose(file);
  return counter;
}

void worker_process(int worker_id, int shmid, char *words[], int words_num) {
  char file_path[MAX_PATH_LENGTH];
  int occurrences[words_num];
  sprintf(file_path, "output/%s%d", OUTPUT_FILES_NAMES, worker_id);

  // Count occurrences of each word in file
  for (int i = 0; i < words_num; i++) {
    occurrences[i] = count_occurrences(file_path, words[i]);
  }

  // Critical section
  char *sharedMsg = (char *)shmat(shmid, NULL, 0);
  sprintf(sharedMsg, "%s", TERMINATION_FLAG);

  for (int i = 0; i < words_num; i++) {
    sprintf(sharedMsg, "%s %d", sharedMsg, occurrences[i]);
  }

  // printf("Message sent from \x1B[32mworker %d\x1B[37m: %s\n", worker_id,
  //        sharedMsg);
  shmdt(sharedMsg);
  printf("\x1B[31mWorker %d\x1B[37m process terminated successfully.\n",
         worker_id);
}
