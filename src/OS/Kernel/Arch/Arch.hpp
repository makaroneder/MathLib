#ifndef Arch_H
#define Arch_H
#include <Typedefs.hpp>
#include <MainData.h>

[[nodiscard]] bool InitArch(uintptr_t signature, void* info);
[[noreturn]] void ArchPanic(void);
[[noreturn]] void ShutdownArch(void);
void ArchSetInterrupts(bool value);
[[nodiscard]] size_t ArchAddTask(uintptr_t address, const MainData* data);
[[nodiscard]] bool ArchRemoveTask(size_t task);
[[nodiscard]] size_t ArchGetCurrentTask(void);
[[nodiscard]] MathLib::num_t ArchSqrt(MathLib::num_t x);
[[nodiscard]] MathLib::num_t GenericSqrt(MathLib::num_t x);
[[nodiscard]] MathLib::num_t ArchInversedTan2(MathLib::num_t y, MathLib::num_t x);
[[nodiscard]] MathLib::num_t GenericInversedTan2(MathLib::num_t y, MathLib::num_t x);
[[nodiscard]] MathLib::num_t ArchLn(MathLib::num_t x);
[[nodiscard]] MathLib::num_t GenericLn(MathLib::num_t x);
[[nodiscard]] MathLib::num_t ArchExp(MathLib::num_t x);
[[nodiscard]] MathLib::num_t GenericExp(MathLib::num_t x);

#endif