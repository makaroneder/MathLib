#ifdef __x86_64__
#ifndef Registers_H
#define Registers_H
#include <Emulator/X86/X86State.hpp>

struct Registers {
    uintptr_t ds;
    uintptr_t es;
    uintptr_t fs;
    uintptr_t gs;
    uintptr_t ss;
    uintptr_t a;
    uintptr_t b;
    uintptr_t c;
    uintptr_t d;
    uintptr_t si;
    uintptr_t di;
    uintptr_t sp;
    uintptr_t bp;
    uintptr_t r8;
    uintptr_t r9;
    uintptr_t r10;
    uintptr_t r11;
    uintptr_t r12;
    uintptr_t r13;
    uintptr_t r14;
    uintptr_t r15;
    uintptr_t ip;
    uintptr_t cs;
    uintptr_t flags;
    uintptr_t ksp;
    uintptr_t kss;

    [[nodiscard]] MathLib::X86State ToX86State(void) const;
} __attribute__((packed));

#endif
#endif