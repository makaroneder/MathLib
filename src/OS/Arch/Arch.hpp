#ifndef Arch_H
#define Arch_H
#include <FunctionPointer.hpp>
#include <Typedefs.hpp>

[[nodiscard]] bool InitArch(uintptr_t signature, void* info);
[[noreturn]] void ArchPanic(void);
[[noreturn]] void ShutdownArch(void);
[[nodiscard]] bool ArchCreateTask(const MathLib::FunctionPointer<void, size_t>& task);
[[nodiscard]] bool ArchDestroyTask(size_t task);
[[nodiscard]] MathLib::num_t ArchSqrt(MathLib::num_t x);
[[nodiscard]] MathLib::num_t GenericSqrt(MathLib::num_t x);
[[nodiscard]] MathLib::num_t ArchInversedTan2(MathLib::num_t y, MathLib::num_t x);
[[nodiscard]] MathLib::num_t GenericInversedTan2(MathLib::num_t y, MathLib::num_t x);
[[nodiscard]] MathLib::num_t ArchLn(MathLib::num_t x);
[[nodiscard]] MathLib::num_t GenericLn(MathLib::num_t x);
[[nodiscard]] MathLib::num_t ArchExp(MathLib::num_t x);
[[nodiscard]] MathLib::num_t GenericExp(MathLib::num_t x);

#endif