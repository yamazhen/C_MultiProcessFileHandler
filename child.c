#include "child.h"

void child_process(int id, int read_pipe[], int write_pipe[]) {
  char buffer[BUFFER_SIZE];
  ssize_t bytes_read;
  char output_dir[] = "output";
  char output_filename[20];
  FILE *output_file;

  struct stat st = {0};
  if (stat(output_dir, &st) == -1) {
    mkdir(output_dir, 0700);
  }

  // create unique output filename for this child
  sprintf(output_filename, "%s/output_%d.txt", output_dir, id);

  // read data from parent through pipe
  bytes_read = read(read_pipe[READ_END], buffer, BUFFER_SIZE);
  if (bytes_read <= 0) {
    fprintf(stderr, "Child %d: Read error or no data\n", id);
    return;
  }

  // null terminate the string
  buffer[bytes_read] = '\0';

  // process data differently based on child id
  switch (id) {
  case 0:
    // first child: uppercase conversion
    for (int i = 0; i < bytes_read; i++) {
      if (buffer[i] >= 'a' && buffer[i] <= 'z') {
        buffer[i] = buffer[i] - 'a' + 'A';
      }
    }
    break;
  case 1:
    // second child: lowercase conversion
    for (int i = 0; i < bytes_read; i++) {
      if (buffer[i] >= 'A' && buffer[i] <= 'Z') {
        buffer[i] = buffer[i] - 'A' + 'a';
      }
    }
    break;
  case 2:
    // third child: reverse the text
    for (int i = 0; i < bytes_read / 2; i++) {
      char temp = buffer[i];
      buffer[i] = buffer[bytes_read - i - 1];
      buffer[bytes_read - i - 1] = temp;
    }
    break;
  }

  // write results to output file
  output_file = fopen(output_filename, "w");
  if (output_file == NULL) {
    perror("File opening failed");
    return;
  }
  fprintf(output_file, "%s", buffer);
  fclose(output_file);

  // send confirmation to parent through pipe
  sprintf(buffer, "Child %d processed data and wrote to %s", id,
          output_filename);
  write(write_pipe[WRITE_END], buffer, strlen(buffer));

  // close pipe ends
  close(read_pipe[READ_END]);
  close(write_pipe[WRITE_END]);
}
