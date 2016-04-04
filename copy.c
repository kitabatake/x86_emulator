#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* メモリは1MB */
#define MEMORY_SIZE (1024 * 1024)

enum Register {
  EAX,
  ECX,
  EDX,
  EBX,
  ESP,
  EBP,
  ESI,
  EDI,
  REGISTERS_COUNT
};

char* registers_name[] = {
    "EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"};

typedef struct {
  uint32_t registers[REGISTERS_COUNT];
  uint32_t eflags;
  uint8_t* memory;
  uint32_t eip;
} Emulator;

static void dump_registers (Emulator* emu)
{
  int i;
  for (i = 0; i < REGISTERS_COUNT; i++) {
    printf("%s => %08x\n", registers_name[i], emu->registers[i]);
  }

  printf("EIP => %08x\n", emu->eip);
}

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

typedef void instruction_func_t(Emulator*);
instruction_func_t* instructions[256];

void init_instructions (void)
{
  int i;
  memset(instructions, 0, sizeof(instructions));

  for (i = 0; i < 8; i++) {
    instructions[0xB8 + i] = mov_r32_imm32; // 汎用レジスタへの既値設定
  }
  instructions[0xEB] = short_jump;
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