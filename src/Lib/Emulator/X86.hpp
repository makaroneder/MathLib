#ifndef Emulator_X86_H
#define Emulator_X86_H
#include "Emulator.hpp"
#include "Register.hpp"
#include "Flags.hpp"

// TODO: Handle segments
struct X86State : Printable {
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

    X86State(Flags flags, Register ip);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual String ToString(const String& padding = "") const override;
};
struct X86 : Emulator {
    X86(const Array<uint8_t>& mem, const X86State& state);
    virtual bool Run(void) override;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual String ToString(const String& padding = "") const override;

    private:
    X86State state;

    bool Step(void);
    void UpdateFlags(uint64_t val, uint64_t a, uint64_t b);
    template <typename T>
    Expected<T> Fetch(void) {
        const Expected<T> ret = ReadPositioned<T>(state.ip.value);
        if (ret.HasValue()) state.ip.value += sizeof(T);
        return ret;
    }
    template <typename T>
    bool Push(T value) {
        state.sp.value -= sizeof(T);
        return WritePositioned<T>(value, state.sp.value);
    }
    template <typename T>
    Expected<T> Pop(void) {
        const Expected<T> ret = ReadPositioned<T>(state.sp.value);
        if (ret.HasValue()) state.sp.value += sizeof(T);
        return ret;
    }
    Register* GetRegister(const uint8_t& code);
};

#endif