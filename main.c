#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* メモリは1MB */
#define MEMORY_SIZE (1024 * 1024)

#include "emulator.h"
#include "emulator_function.h"
#include "instruction.h"

Emulator* create_emu (size_t size, uint32_t eip, uint32_t esp)
{
  Emulator* emu = malloc(sizeof(Emulator));
  emu->memory = malloc(size);

  memset(emu->registers, 0, sizeof(emu->registers));
  emu->eip = eip;
  emu->registers[ESP] = esp;
  return emu;
}

void destroy_emu (Emulator* emu)
{
  free(emu->memory);
  free(emu);
}


char* registers_name[] = {
    "EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"};

static void dump_registers (Emulator* emu)
{
  int i;
  for (i = 0; i < REGISTERS_COUNT; i++) {
    printf("%s => %08x\n", registers_name[i], emu->registers[i]);
  }

  printf("EIP => %08x\n", emu->eip);
}

int main (int argc, char* argv[])
{
  FILE* binary;
  Emulator* emu;
  emu = create_emu(MEMORY_SIZE, 0x0000, 0x7c00);

  binary = fopen(argv[1], "rb");

  fread(emu->memory, 1, 0x200, binary);
  fclose(binary);

  init_instructions();

  while (emu->eip < MEMORY_SIZE) {
    uint8_t code = get_code8(emu, 0);
    printf("EIP = %d, code = %02x\n", emu->eip, code);

    if (instructions[code] == NULL) {
      printf("\n\nNot Implemented: %x\n", code);
    }

    instructions[code](emu);
    if (emu->eip == 0x00) {
      printf("\n\nend program!\n");
      break;
    }
  }

  dump_registers(emu);
  destroy_emu(emu);
  return 0;
}