#ifdef __x86_64__
#ifndef PS2_H
#define PS2_H
#include <stdint.h>

[[nodiscard]] bool Await8042(bool write);
void InitPS2(void);

#endif
#endif