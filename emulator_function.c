#include "emulator_function.h"

uint8_t get_code8 (Emulator* emu, int index)
{
  return emu->memory[emu->eip + index];
}

int8_t get_sign_code8 (Emulator* emu, int index)
{
  return (int8_t) emu->memory[emu->eip + index];
}

int32_t get_sign_code32 (Emulator* emu, int index)
{
  return (int32_t) get_code32(emu, index);
}


uint32_t get_code32 (Emulator* emu, int index)
{
  int i = 0;
  uint32_t ret = 0;

  // リトルエンディアン処理.
  for (i = 0; i < 4; i++) {
    ret |= get_code8(emu, index + i) << (i * 8);
  }

  return ret;
}

/* index番目の32bit汎用レジスタに値を設定する */
void set_register32(Emulator* emu, int index, uint32_t value)
{
  emu->registers[index] = value;
}

uint32_t get_register32(Emulator* emu, uint8_t reg_index)
{
  return emu->registers[reg_index];
}

/* メモリのindex番地に8bit値を設定する */
void set_memory8(Emulator* emu, uint32_t address, uint8_t value)
{
  emu->memory[address] = value;
}

/* メモリのindex番地に32bit値を設定する */
void set_memory32(Emulator* emu, uint32_t address, uint32_t value)
{
  int i;
  for (i = 0; i < 4; i++) {
    emu->memory[address + i] = 0xFF & (value >> (i * 8));
  }
}

uint32_t get_memory8(Emulator* emu, uint32_t address)
{
    return emu->memory[address];
}

uint32_t get_memory32(Emulator* emu, uint32_t address)
{
    int i;
    uint32_t ret = 0;

    /* リトルエンディアンでメモリの値を取得する */
    for (i = 0; i < 4; i++) {
        ret |= get_memory8(emu, address + i) << (8 * i);
    }

    return ret;
}