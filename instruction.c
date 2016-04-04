#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "instruction.h"
#include "emulator.h"
#include "emulator_function.h"

instruction_func_t* instructions[256];

void short_jump (Emulator* emu) {
  int diff = get_sign_code8(emu, 1);
  emu->eip += diff + 2;
}

void mov_r32_imm32 (Emulator* emu) {
  int8_t reg = get_code8(emu, 0) - 0xB8;
  int32_t value = get_code32(emu, 1);

  printf("move %x => %08x\n", reg, value);

  emu->registers[reg] = value;
  emu->eip += 5;
}

void init_instructions (void)
{
  int i;
  memset(instructions, 0, sizeof(instructions));

  for (i = 0; i < 8; i++) {
    instructions[0xB8 + i] = mov_r32_imm32; // 汎用レジスタへの既値設定
  }
  instructions[0xEB] = short_jump;
}