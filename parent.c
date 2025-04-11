#include "parent.h"

void parent_process(int read_pipes[][2], int write_pipes[][2],
                    int num_children) {
  char buffer[BUFFER_SIZE];
  ssize_t bytes_read;
  FILE *input_file;

  // open input file or create if it doesn't exist
  input_file = fopen("input.txt", "r");
  if (input_file == NULL) {
    input_file = fopen("input.txt", "w");
    const char *default_content = "First line\nSecond line\nThird line\n";
    fprintf(input_file, "%s", default_content);
    fclose(input_file);
    input_file = fopen("input.txt", "r");
  }

  // read the entire input file
  bytes_read = fread(buffer, 1, BUFFER_SIZE - 1, input_file);
  fclose(input_file);

  // null terminate the string
  buffer[bytes_read] = '\0';

  printf("Parent: Read %zd bytes from input file\n", bytes_read);

  // send data to all children through pipes
  for (int i = 0; i < num_children; i++) {
    write(write_pipes[i][WRITE_END], buffer, bytes_read);
    printf("Parent: Sent data to child %d\n", i);
  }

  // receive results from all children
  for (int i = 0; i < num_children; i++) {
    bytes_read = read(read_pipes[i][READ_END], buffer, BUFFER_SIZE);
    buffer[bytes_read] = '\0';
    printf("Parent received: %s\n", buffer);
  }

  // close all pipe ends
  for (int i = 0; i < num_children; i++) {
    close(write_pipes[i][WRITE_END]);
    close(read_pipes[i][READ_END]);
  }
}
