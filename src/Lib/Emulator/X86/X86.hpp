#ifndef Emulator_X86_H
#define Emulator_X86_H
#include "../Emulator.hpp"
#include "X86State.hpp"

enum class X86RegisterCode : uint8_t {
    A,
    C,
    D,
    B,
    SP,
    BP,
    SI,
    DI,
};
// TODO: test xchg div idiv imul loop loope loopne loopnz loopz mul neg not rcl rcr rol ror sal
// TODO: sar rep repe repne repnz repz in out
enum class X86Opcode : uint8_t {
    Add16 = 0x01,
    Or16 = 0x09,
    Extended = 0x0f,
    Adc16 = 0x11,
    Sbb16 = 0x19,
    And16 = 0x21,
    Daa = 0x27,
    Sub16 = 0x29,
    Das = 0x2f,
    Xor16 = 0x31,
    Aaa = 0x37,
    Cmp16 = 0x39,
    Aas = 0x3f,
    Inc16Start,
    Inc16End = 0x47,
    Dec16Start,
    Dec16End = 0x4f,
    Push16Start,
    Push16End = 0x57,
    Pop16Start,
    Pop16End = 0x5f,
    Pusha16,
    Popa16,
    Jo8 = 0x70,
    Jno8,
    Jc8,
    Jb8 = Jc8, 
    Jnae8 = Jc8,
    Jnc8,
    Jnb8 = Jnc8,
    Jae8 = Jnc8,
    Jz8,
    Je8 = Jz8,
    Jnz8,
    Jne8 = Jnz8,
    Jbe8,
    Jna8 = Jbe8,
    Ja8,
    Jnbe8 = Ja8,
    Js8,
    Jns8,
    Jp8,
    Jpe8 = Jp8,
    Jnp8,
    Jpo8 = Jnp8,
    Jl8,
    Jnge8 = Jl8,
    Jge8,
    Jnl8 = Jge8,
    Jle8,
    Jng8 = Jle8,
    Jg8,
    Jnle8 = Jg8,
    ExtendedJc16 = 0x82,
    ExtendedJnc16,
    ExtendedJz16,
    ExtendedJe16 = ExtendedJz16,
    ExtendedJnz16,
    ExtendedJne16 = ExtendedJnz16,
    Lea = 0x8d,
    Nop = 0x90,
    Cbw = 0x98,
    Cwd,
    Pushf16 = 0x9c,
    Popf16,
    Sahf,
    Lahf,
    Movs8 = 0xa4,
    Cmps8 = 0xa6, // TODO:
    Cmps16, // TODO:
    Stos8 = 0xaa,
    Lods8 = 0xac,
    Scas8 = 0xae, // TODO:
    Scas16, // TODO:
    Mov8Start,
    Mov8End = 0xb7,
    Mov16Start,
    Mov16End = 0xbf,
    Ret16 = 0xc3,
    Les = 0xc4, // TODO:
    Lds, // TODO:
    ImmediateRetf16 = 0xca,
    Retf16,
    Int3, // TODO:
    Int, // TODO:
    Into, // TODO:
    IRet16,
    Shr16 = 0xd3,
    Shl16 = Shr16,
    Aam,
    Aad,
    Xlat8 = 0xd7,
    Jcxz8 = 0xe3,
    Call16 = 0xe8,
    Jmp16,
    Jmp8 = 0xeb,
    Hlt = 0xf4,
    Cmc,
    Clc = 0xf8,
    Stc,
    Cli,
    Sti,
    Cld,
    Std,
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
    Register* GetRegister(uint8_t code);
};

#endif