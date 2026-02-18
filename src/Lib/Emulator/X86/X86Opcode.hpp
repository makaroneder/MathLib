#ifndef MathLib_Emulator_X86_Opcode_H
#define MathLib_Emulator_X86_Opcode_H
#include <stdint.h>

// TODO: test (rcl rcr rol ror sal sar) in out esc

// TODO: 38, 3a-3d, 62-63, 66-67, 69, 6b-6f, 88-8c, 8e, 9a-9b, a0-a3, a8-a9, c0-c1, c6-c9,
// TODO: d0-d2, d6, d8-df, e4-e7, ea-eb, ec-f1
namespace MathLib {
    enum class X86Opcode : uint8_t {
        AddAnyReg8 = 0x00,
        AddAnyReg16,
        AddRegAny8, // TODO:
        AddRegAny16, // TODO:
        AddAImmediate8, // TODO:
        AddAImmediate16, // TODO:
        PushES16,
        PopES16,
        OrAnyReg8,
        OrAnyReg16,
        OrRegAny8, // TODO:
        OrRegAny16, // TODO:
        OrAImmediate8, // TODO:
        OrAImmediate16, // TODO:
        PushCS16,
        Extended,
        AdcAnyReg8,
        AdcAnyReg16,
        AdcRegAny8, // TODO:
        AdcRegAny16, // TODO:
        AdcAImmediate8, // TODO:
        AdcAImmediate16, // TODO:
        PushSS16,
        PopSS16,
        SbbAnyReg8,
        SbbAnyReg16,
        SbbRegAny8, // TODO:
        SbbRegAny16, // TODO:
        SbbAImmediate8, // TODO:
        SbbAImmediate16, // TODO:
        PushDS16,
        PopDS16,
        AndAnyReg8,
        AndAnyReg16,
        AndRegAny8, // TODO:
        AndRegAny16, // TODO:
        AndAImmediate8, // TODO:
        AndAImmediate16, // TODO:
        UseES, // TODO:
        Daa,
        SubAnyReg8,
        SubAnyReg16,
        SubRegAny8, // TODO:
        SubRegAny16, // TODO:
        SubAImmediate8, // TODO:
        SubAImmediate16, // TODO:
        UseCS, // TODO:
        Das,
        XorAnyReg8,
        XorAnyReg16,
        XorRegAny8, // TODO:
        XorRegAny16, // TODO:
        XorAImmediate8, // TODO:
        XorAImmediate16, // TODO:
        UseSS, // TODO:
        Aaa,
        Cmp16 = 0x39,
        UseDS = 0x3e, // TODO:
        Aas,
        Inc16Start,
        Inc16End = 0x47,
        Dec16Start,
        Dec16End = 0x4f,
        PushReg16Start,
        PushReg16End = 0x57,
        PopReg16Start,
        PopReg16End = 0x5f,
        Pusha16,
        Popa16,
        UseFS = 0x64, // TODO:
        UseGS, // TODO:
        PushImmediate16 = 0x68, // TODO:
        PushImmediate8 = 0x6a, // TODO:
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
        AddAnyImmediate8 = 0x80, // TODO:
        OrAnyImmediate8 = AddAnyImmediate8,
        AndAnyImmediate8 = AddAnyImmediate8,
        XorAnyImmediate8 = AddAnyImmediate8,
        SubAnyImmediate8 = AddAnyImmediate8,
        AdcAnyImmediate8 = AddAnyImmediate8,
        SbbAnyImmediate8 = AddAnyImmediate8,
        AddAnyImmediate16, // TODO:
        OrAnyImmediate16 = AddAnyImmediate16,
        AndAnyImmediate16 = AddAnyImmediate16,
        XorAnyImmediate16 = AddAnyImmediate16,
        SubAnyImmediate16 = AddAnyImmediate16,
        AdcAnyImmediate16 = AddAnyImmediate16,
        SbbAnyImmediate16 = AddAnyImmediate16,
        ExtendedJc16,
        AddAny16Immediate8, // TODO:
        OrAny16Immediate8 = AddAny16Immediate8,
        AndAny16Immediate8 = AddAny16Immediate8,
        XorAny16Immediate8 = AddAny16Immediate8,
        SubAny16Immediate8 = AddAny16Immediate8,
        AdcAny16Immediate8 = AddAny16Immediate8,
        SbbAny16Immediate8 = AddAny16Immediate8,
        ExtendedJnc16 = AddAny16Immediate8,
        ExtendedJz16,
        ExtendedJe16 = ExtendedJz16,
        ExtendedJnz16,
        ExtendedJne16 = ExtendedJnz16,
        Xchg8,
        XchgRegAddr16,
        Lea = 0x8d,
        PopAddr = 0x8f, // TODO:
        Xchg16Start,
        Nop = Xchg16Start,
        Xchg16End = 0x97,
        Cbw,
        Cwd,
        Wait = 0x9b, // TODO:
        Pushf16,
        Popf16,
        Sahf,
        Lahf,
        Movs8 = 0xa4,
        Movs16,
        Cmps8,
        Cmps16,
        Stos8 = 0xaa,
        Stos16,
        Lods8,
        Lods16,
        Scas8,
        Scas16,
        Mov8Start,
        Mov8End = 0xb7,
        Mov16Start,
        Mov16End = 0xbf,
        ImmediateRet16 = 0xc2,
        Ret16,
        Les,
        Lds,
        ImmediateRetf16 = 0xca,
        Retf16,
        Int3,
        Int,
        Into,
        IRet16,
        Shr16 = 0xd3,
        Shl16 = Shr16,
        Aam,
        Aad,
        Xlat8 = 0xd7,
        Loopne = 0xe0,
        Loopnz = Loopne,
        Loope,
        Loopz = Loope,
        Loop,
        Jcxz8,
        Call16 = 0xe8,
        Jmp16,
        Jmp8 = 0xeb,
        Lock = 0xf0, // TODO:
        Repne = 0xf2,
        Repnz = Repne,
        Rep,
        Repe = Rep,
        Repz = Rep,
        Hlt,
        Cmc,
        Neg8,
        Not8 = Neg8,
        Div8 = Neg8, // TODO:
        IDiv8 = Neg8, // TODO:
        Mul8 = Neg8, // TODO:
        IMul8 = Neg8, // TODO:
        Neg16,
        Not16 = Neg16,
        Div16 = Neg16, // TODO:
        IDiv16 = Neg16, // TODO:
        Mul16 = Neg16, // TODO:
        IMul16 = Neg16, // TODO:
        Clc,
        Stc,
        Cli,
        Sti,
        Cld,
        Std,
        IncAddr8, // TODO:
        DecAddr8 = IncAddr8, // TODO:
        IncAddr16, // TODO:
        PushAddr = IncAddr16, // TODO:
        DecAddr16 = IncAddr16, // TODO:
    };
}

#endif