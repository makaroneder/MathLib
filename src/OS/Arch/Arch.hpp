#ifndef Arch_H
#define Arch_H
#include <stdint.h>

bool InitArch(uintptr_t signature, void* info);
[[noreturn]] void ArchPanic(void);

#endif