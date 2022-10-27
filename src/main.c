#include <stdio.h>    // used for printf()
#include <stdlib.h>   // used for exit()
#include <string.h>   // used for strstr()
#include <sys/shm.h>  // used for shmget(), shmat(), ...
#include <sys/stat.h> // used for S_IRUSR and S_IWUSR
#include <sys/types.h>
#include <sys/wait.h> // used for wait(), WEXITSTATUS
#include <unistd.h>   // used for fork(), usleep()

#define INPUT_FILE_NAME "input.txt" //"test"
#define OUTPUT_FILES_NAMES "output"
#define OUTPUT_FILES_LENGTH 80000
#define N 4

#define TERMINATION_FLAG "999"
#define SHMSZ 1000
#define MAX_PATH_LENGTH 128
#define MAX_COMMAND_LENGTH 1024
#define BUFFER_SIZE 1024

void wait_for_input(char *msg) {
  char buffer[100];
  printf("%s", msg);
  fgets(buffer, 100, stdin);
}

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

void reducer_process(int reducer_id, int shmids[], char *words[],
                     int words_num) {
  char *sharedMsg;
  int word_counter = 0;
  // For each worker process
  for (int i = 0; i < N; i++) {
    sharedMsg = (char *)shmat(shmids[i], NULL, 0);
    while (strncmp(sharedMsg, TERMINATION_FLAG, strlen(TERMINATION_FLAG)) != 0)
      ;
    // printf("Message read by \x1B[33mreducer %d\x1B[37m from worker %d: %s\n",
    //        reducer_id, i, sharedMsg);

    int occurrences[words_num];
    char *ptr = sharedMsg + strlen(TERMINATION_FLAG) - 1;
    for (int j = 0; j < words_num; j++) {
      ptr = strstr(ptr + 1, " ");
      sscanf(ptr, " %d", &occurrences[j]);
    }
    shmdt(sharedMsg);
    word_counter += occurrences[reducer_id];
    printf("\x1B[34mReducer %d\x1B[37m read worker %d\n", reducer_id, i);
  }
  printf("\x1B[34mReducer %d\x1B[37m process terminated successfully.\n",
         reducer_id);
  printf("\t%s counter: %d\n", words[reducer_id], word_counter);
}

void splitter_process() {
  system("mkdir -p output");
  char command[MAX_COMMAND_LENGTH];
  sprintf(command, "split -l %d -a 1 -d input/%s output/%s",
          OUTPUT_FILES_LENGTH, INPUT_FILE_NAME, OUTPUT_FILES_NAMES);
  if (!system(command)) {
    printf("\x1B[33mSplitter\x1B[37m process terminated successfully.\n");
  } else {
    printf("ERROR: Splitting failed!");
  }
}

int main() {
  int status;
  int shmids[N];
  char *words[] = {"CMPS", "CCE", "ECE"};
  int words_num = sizeof(words) / sizeof(char *);

  // Splitting child
  pid_t splitting_child_pid = fork();
  if (splitting_child_pid == 0) {
    splitter_process();
    exit(0);
  }

  // Parent
  waitpid(splitting_child_pid, &status, 0);
  wait_for_input("Press enter to read the files...\n");

  // Create N shared memories
  for (int i = 0; i < N; i++) {
    shmids[i] = shmget(IPC_PRIVATE, 4 * sizeof(long), S_IRUSR | S_IWUSR);
  }

  // Fork N Worker processes
  pid_t workers_pid[N];
  for (int i = 0; i < N; i++) {
    workers_pid[i] = fork();
    if (workers_pid[i] == 0) {
      worker_process(i, shmids[i], words, words_num);
      exit(0);
    }
  }

  // Wait for the workers
  // for (int i = 0; i < N; i++) {
  //   waitpid(workers_pid[i], &status, 0);
  // }

  // Fork Reducer processes (one for each word)
  pid_t reducers_pid[words_num];
  for (int word_index = 0; word_index < words_num; word_index++) {
    reducers_pid[word_index] = fork();
    if (reducers_pid[word_index] == 0) {
      reducer_process(word_index, shmids, words, words_num);
      exit(0);
    }
  }

  // Wait for the reducers
  for (int i = 0; i < words_num; i++) {
    waitpid(reducers_pid[i], &status, 0);
    printf("Reducer %d exit status: %d\n", i, status);
  }

  wait_for_input("Press enter to delete the output files...\n");

  // Delete the files
  if (system("rm -r output") != 0) {
    printf("Deleting failed!\n");
  }

  return 0;
}