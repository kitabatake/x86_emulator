#include "emulator_function.h"

uint8_t get_code8 (Emulator* emu, int index)
{
  return emu->memory[emu->eip + index];
}

int8_t get_sign_code8 (Emulator* emu, int index)
{
  return (int8_t) emu->memory[emu->eip + index];
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