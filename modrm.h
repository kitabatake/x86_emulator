#ifndef MODRM_H_
#define MODRM_H_

#include <stdint.h>

#include "emulator.h"

typedef struct {
  uint8_t mod;
  union {
    uint8_t opecode;
    uint8_t reg_index;
  };
  uint8_t rm;

  uint8_t sib;
  union {
    int8_t disp8;
    uint32_t disp32;
  };
} ModRM;

/* ModR/M, SIB, ディスプレースメントを解析する
 *
 * emu から ModR/M, SIB, ディスプレースメントを読み取って modrm にセットする。
 * 呼び出しのとき emu->eip は ModR/M バイトを指している必要がある。
 * この関数は emu->eip を即値（即値がない場合は次の命令）の先頭を指すように変更する。
 *
 * 引数
 *   emu: eip が ModR/M バイトの先頭を指しているエミュレータ構造体
 *   modrm: 解析結果を格納する構造体
 */
void parse_modrm(Emulator* emu, ModRM* modrm);

void set_rm32(Emulator* emu, ModRM* modrm, uint32_t value);

#endif