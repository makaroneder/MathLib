#ifndef Emulator_X86_H
#define Emulator_X86_H
#include "Emulator.hpp"
#include "Register.hpp"
#include "Flags.hpp"

struct X86 : Emulator {
    X86(const Array<uint8_t>& mem);
    virtual bool Step(void) override;
    Register GetIP(void) const;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual String ToString(const String& padding = "") const override;

    private:
    void UpdateFlags(uint64_t val, uint64_t a, uint64_t b);
    bool Fetch(uint8_t& out);
    bool Push(uint8_t val);
    uint8_t Pop(void);
    Register* GetRegister(const uint8_t& code);

    Flags flags;
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