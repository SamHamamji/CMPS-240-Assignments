#include <stdio.h>  // used for printf()
#include <stdlib.h> // used for exit()

#include "../include/Constants.h"

void splitter_process(char *input_file_name, int output_files_length) {
  system("mkdir -p output");
  char command[MAX_COMMAND_LENGTH];
  sprintf(command, "split -l %d -a 1 -d input/%s output/%s",
          output_files_length, input_file_name, OUTPUT_FILES_NAMES);
  if (!system(command)) {
    printf("\x1B[%dmSplitter\x1B[37m process terminated successfully.\n",
           SPLITTER_COLOR_ID);
  } else {
    printf("ERROR: Splitting failed!");
  }
}
