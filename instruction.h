#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "emulator.h"

void init_instructions(void);
typedef void instruction_func_t(Emulator*);
extern instruction_func_t* instructions[256];

#endif