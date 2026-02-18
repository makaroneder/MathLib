#ifdef __x86_64__
#include "Registers.hpp"
#include <String.hpp>

Registers::Registers(void) : sp(0), ds(0), es(0), fs(0), gs(0), ss(0), a(0), b(0), c(0), d(0), si(0), di(0), bp(0), r8(0), r9(0), r10(0), r11(0), r12(0), r13(0), r14(0), r15(0), ip(0), cs(0), flags(0), ksp(0), kss(0) {}
MathLib::X86State Registers::ToX86State(void) const {
    MathLib::X86State ret = MathLib::X86State(flags, ip);
    ret.cs = cs;
    ret.ds = ds;
    ret.es = es;
    ret.fs = fs;
    ret.gs = gs;
    ret.ss = ss;
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

#endif