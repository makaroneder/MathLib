#ifndef Arch_H
#define Arch_H
#include <FunctionPointer.hpp>
#include <stdint.h>

bool InitArch(uintptr_t signature, void* info);
[[noreturn]] void ArchPanic(void);
[[noreturn]] void ShutdownArch(void);
bool ArchCreateTask(const MathLib::FunctionPointer<void, size_t>& task);
bool ArchDestroyTask(size_t task);

#endif