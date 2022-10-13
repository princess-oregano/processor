#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>

const int N_REG = 5;
const int N_RAM = 15;

void
execute(int *cmd_buf, size_t size);

#endif // PROCESS_H

