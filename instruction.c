#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "instruction.h"
#include "emulator.h"
#include "emulator_function.h"

#include "modrm.h"

instruction_func_t* instructions[256];

// オペコードブンのeipが進められた状態で呼びだされ、処理後はeipが次のオペコードを指すようにする責任を持つ

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

void mov_rm32_imm32 (Emulator* emu) {
  emu->eip += 1;
  ModRM modrm;
  parse_modrm(emu, &modrm);
  uint32_t value = get_code32(emu,0);
  emu->eip += 4;
  set_rm32(emu, &modrm, value);
}

static void mov_r32_rm32(Emulator* emu)
{
    emu->eip += 1;
    ModRM modrm;
    parse_modrm(emu, &modrm);
    uint32_t rm32 = get_rm32(emu, &modrm);
    set_register32(emu, modrm.reg_index, rm32);
}


void init_instructions (void)
{
  int i;
  memset(instructions, 0, sizeof(instructions));

  for (i = 0; i < 8; i++) {
    instructions[0xB8 + i] = mov_r32_imm32; // 汎用レジスタへの既値設定
  }
  instructions[0xEB] = short_jump;
  instructions[0xC7] = mov_rm32_imm32;
  instructions[0x8B] = mov_r32_rm32;
}