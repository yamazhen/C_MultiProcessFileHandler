#include "child.h"
#include "common.h"
#include "parent.h"

int main() {
  pid_t pid;
  int i;

  // pipes for communication between processes
  int parent_to_child[NUM_CHILDREN][2];
  int child_to_parent[NUM_CHILDREN][2];

  // create the communication pipes
  for (i = 0; i < NUM_CHILDREN; i++) {
    if (pipe(parent_to_child[i]) < 0 || pipe(child_to_parent[i]) < 0) {
      perror("Pipe creation failed");
      exit(EXIT_FAILURE);
    }
  }

  // create multiple child processes with fork()
  for (i = 0; i < NUM_CHILDREN; i++) {
    pid = fork();

    if (pid < 0) {
      perror("Fork failed");
      exit(EXIT_FAILURE);
    } else if (pid == 0) {
      // CHILD PROCESS CODE
      // close unused pipe ends to prevent leaks
      for (int j = 0; j < NUM_CHILDREN; j++) {
        if (j != i) {
          // close other children's pipes
          close(parent_to_child[j][READ_END]);
          close(parent_to_child[j][WRITE_END]);
          close(child_to_parent[j][READ_END]);
          close(child_to_parent[j][WRITE_END]);
        } else {
          // close unused ends of this child's pipes
          close(parent_to_child[j][WRITE_END]);
          close(child_to_parent[j][READ_END]);
        }
      }

      // run child's specific processing function
      child_process(i, parent_to_child[i], child_to_parent[i]);
      exit(EXIT_SUCCESS);
    }
  }

  // PARENT PROCESS CODE
  // parent closes unused pipe ends
  for (i = 0; i < NUM_CHILDREN; i++) {
    close(parent_to_child[i][READ_END]);
    close(child_to_parent[i][WRITE_END]);
  }

  // execute parent process function
  parent_process(child_to_parent, parent_to_child, NUM_CHILDREN);

  // wait for all children to finish
  for (i = 0; i < NUM_CHILDREN; i++) {
    wait(NULL);
  }

  printf("All processes completed successfully\n");
  return 0;
}
