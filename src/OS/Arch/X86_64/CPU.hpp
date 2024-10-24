#ifdef __x86_64__
#ifndef CPU_H
#define CPU_H
#include <stdint.h>

uintptr_t GetFlags(void);
void SetFlags(uintptr_t flags);

#endif
#endif