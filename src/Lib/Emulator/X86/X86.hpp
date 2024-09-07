#ifndef Emulator_X86_H
#define Emulator_X86_H
#include "../Emulator.hpp"
#include "X86State.hpp"

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
    uint64_t ToLinear(uint64_t segment, uint64_t offset);
    template <typename T>
    Expected<T> Fetch(void) {
        const Expected<T> ret = ReadPositioned<T>(ToLinear(state.cs.value, state.ip.value));
        if (ret.HasValue()) state.ip.value += sizeof(T);
        return ret;
    }
    template <typename T>
    bool Push(T value) {
        state.sp.value -= sizeof(T);
        return WritePositioned<T>(value, ToLinear(state.ss.value, state.sp.value));
    }
    template <typename T>
    Expected<T> Pop(void) {
        const Expected<T> ret = ReadPositioned<T>(ToLinear(state.ss.value, state.sp.value));
        if (ret.HasValue()) state.sp.value += sizeof(T);
        return ret;
    }
    Register* GetRegister(const uint8_t& code);
};

#endif