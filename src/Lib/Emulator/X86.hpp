#ifndef Emulator_X86_H
#define Emulator_X86_H
#include "Emulator.hpp"
#include "Register.hpp"

struct X86 : Emulator {
    X86(Array<uint8_t> mem);
    virtual bool Step(void) override;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual String ToString(String padding = "") const override;

    private:
    bool Fetch(uint8_t& out);
    Register* GetRegister(uint8_t code);

    Register ip;
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
};

#endif