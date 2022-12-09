#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>

const int N_REG = 16;
const int N_RAM = 300;
// Resolution of screen.
const int RESOL_X = 40;
const int RESOL_Y = 20;

void
execute(double *cmd_buf, size_t size);

#endif // PROCESS_H

