#ifdef __x86_64__
#ifndef Interrupts_H
#define Interrupts_H
#include <stdint.h>

[[nodiscard]] uint8_t GetIRQBase(void);
[[nodiscard]] bool InitInterrupts(uint8_t irqBase, uint8_t codeSegment);

#endif
#endif