#include <stdio.h>    // used for printf()
#include <stdlib.h>   // used for exit()
#include <sys/shm.h>  // used for shmget(), shmat(), ...
#include <sys/stat.h> // used for S_IRUSR and S_IWUSR
#include <sys/wait.h> // used for wait(), WEXITSTATUS
#include <unistd.h>   // used for fork(), usleep()

#include "../include/Constants.h"
#include "../include/reducer.h"
#include "../include/splitter.h"
#include "../include/worker.h"

#define INPUT_FILE_NAME "input.txt"

void wait_for_input(char *msg) {
  char buffer[100];
  printf("%s", msg);
  fgets(buffer, 100, stdin);
}

int get_file_length(char *file_path) {
  int counter = 1;
  FILE *file = fopen(file_path, "r");
  if (file == NULL)
    return -1;
  for (char c = getc(file); c != EOF; c = getc(file)) {
    if (c == '\n') {
      counter = counter + 1;
    }
  }
  fclose(file);
  return counter;
}

int main(int argc, char **argv) {
  // Error handling
  if (argc <= 1) {
    printf("ERROR: No agument has been entered\n");
    return -1;
  }
  const int N = atoi(argv[1]);
  if (N < 1) {
    printf("ERROR: Invalid number of workers\n");
    return -1;
  } else if (N > 9) {
    printf("ERROR: Maximum number of workers (10) exceeded\n");
    return -1;
  }
  const int INPUT_FILE_LENGTH = get_file_length("input/" INPUT_FILE_NAME);
  const int OUTPUT_FILES_LENGTH = 1 + ((INPUT_FILE_LENGTH - 1) / N);
  if (INPUT_FILE_LENGTH == -1) {
    printf("ERROR: Input file does not exist\n");
    return -1;
  }

  // Variable declarations
  int status;
  int shmids[N];
  char *words[] = {"CMPS", "CCE", "ECE"};
  int words_num = sizeof(words) / sizeof(char *);

  // Splitting child
  pid_t splitting_child_pid = fork();
  if (splitting_child_pid == 0) {
    splitter_process(INPUT_FILE_NAME, OUTPUT_FILES_LENGTH);
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

  // Fork Reducer processes (one for each word)
  pid_t reducers_pid[words_num];
  for (int word_index = 0; word_index < words_num; word_index++) {
    reducers_pid[word_index] = fork();
    if (reducers_pid[word_index] == 0) {
      reducer_process(word_index, shmids, words, words_num, N);
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