#ifndef PARENT_H
#define PARENT_H

#include "common.h"

void parent_process(int read_pipes[][2], int write_pipes[][2],
                    int num_children);

#endif
