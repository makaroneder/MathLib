#include "Registers.hpp"
#include <String.hpp>

X86State Registers::ToX86State(void) const {
    X86State ret = X86State(flags, ip);
    ret.a = a;
    ret.b = b;
    ret.c = c;
    ret.d = d;
    ret.si = si;
    ret.di = di;
    ret.sp = sp;
    ret.bp = bp;
    ret.r8 = r8;
    ret.r9 = r9;
    ret.r10 = r10;
    ret.r11 = r11;
    ret.r12 = r12;
    ret.r13 = r13;
    ret.r14 = r14;
    ret.r15 = r15;
    return ret;
}