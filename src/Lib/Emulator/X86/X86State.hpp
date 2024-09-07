#ifndef X86State_H
#define X86State_H
#include "../Register.hpp"
#include "Flags.hpp"

struct X86State : Printable {
    Flags flags;
    Register ip;
    Register cs;
    Register ds;
    Register es;
    Register fs;
    Register gs;
    Register ss;
    Register a;
    Register b;
    Register c;
    Register d;
    Register si;
    Register di;
    Register sp;
    Register bp;
    Register r8;
    Register r9;
    Register r10;
    Register r11;
    Register r12;
    Register r13;
    Register r14;
    Register r15;

    X86State(Flags flags, Register ip);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual String ToString(const String& padding = "") const override;
};

#endif