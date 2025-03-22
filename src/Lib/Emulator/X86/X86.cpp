#include "X86.hpp"
#include "X86ModRM.hpp"
#include "X86RegisterCode.hpp"

namespace MathLib {
    #define ModRMOperation(regAction, addrAction)                                       \
    if (modrm.mod == (uint8_t)X86ModRM::DisplacementMode::Register) {                   \
        Register* reg = GetRegister(modrm.rm);                                          \
        if (!reg) ReturnFromBenchmark(false)                                            \
        regAction                                                                       \
    }                                                                                   \
    else {                                                                              \
        uint16_t addr = 0;                                                              \
        if (modrm.mod == (uint8_t)X86ModRM::DisplacementMode::Bit8) {                   \
            const Expected<uint8_t> tmp = Fetch<uint8_t>();                             \
            if (!tmp.HasValue()) ReturnFromBenchmark(false)                             \
            addr = tmp.Get();                                                           \
        }                                                                               \
        else if (modrm.mod == (uint8_t)X86ModRM::DisplacementMode::Bit16) {             \
            const Expected<uint16_t> tmp = Fetch<uint16_t>();                           \
            if (!tmp.HasValue()) ReturnFromBenchmark(false)                             \
            addr = tmp.Get();                                                           \
        }                                                                               \
        switch ((X86ModRM::AddressingMode)modrm.rm) {                                   \
            case X86ModRM::AddressingMode::BPlusSI: {                                   \
                addr += state.b.Get16(false) + state.si.Get16(false);                   \
                break;                                                                  \
            }                                                                           \
            case X86ModRM::AddressingMode::BPlusDI: {                                   \
                addr += state.b.Get16(false) + state.di.Get16(false);                   \
                break;                                                                  \
            }                                                                           \
            case X86ModRM::AddressingMode::BPPlusSI: {                                  \
                addr += state.bp.Get16(false) + state.si.Get16(false);                  \
                break;                                                                  \
            }                                                                           \
            case X86ModRM::AddressingMode::BPPlusDI: {                                  \
                addr += state.bp.Get16(false) + state.di.Get16(false);                  \
                break;                                                                  \
            }                                                                           \
            case X86ModRM::AddressingMode::SI: {                                        \
                addr += state.si.Get16(false);                                          \
                break;                                                                  \
            }                                                                           \
            case X86ModRM::AddressingMode::DI: {                                        \
                addr += state.di.Get16(false);                                          \
                break;                                                                  \
            }                                                                           \
            case X86ModRM::AddressingMode::BP: {                                        \
                if (modrm.mod == (uint8_t)X86ModRM::DisplacementMode::NoDisplacement) { \
                    const Expected<uint16_t> tmp = Fetch<uint16_t>();                   \
                    if (!tmp.HasValue()) ReturnFromBenchmark(false)                     \
                    addr = tmp.Get();                                                   \
                }                                                                       \
                else addr += state.bp.Get16(false);                                     \
                break;                                                                  \
            }                                                                           \
            case X86ModRM::AddressingMode::BX: {                                        \
                addr += state.b.Get16(false);                                           \
                break;                                                                  \
            }                                                                           \
            default: ReturnFromBenchmark(false)                                         \
        }                                                                               \
        addrAction                                                                      \
    }
    #define AnyRegMathOperation(op, operation, size, cf, of, arg)                           \
    case op: {                                                                              \
        const Expected<uint8_t> tmp = Fetch<uint8_t>();                                     \
        if (!tmp.HasValue()) ReturnFromBenchmark(false)                                     \
        const X86ModRM modrm = tmp.Get();                                                   \
        uint64_t result;                                                                    \
        uint##size##_t a;                                                                   \
        uint##size##_t b;                                                                   \
        ModRMOperation({                                                                    \
            Register* src = GetRegister(modrm.reg);                                         \
            if (!src) ReturnFromBenchmark(false)                                            \
            a = reg->Get##size(false);                                                      \
            b = src->Get##size(false) + arg;                                                \
            result = a operation b;                                                         \
            reg->Set##size(result, false);                                                  \
        }, {                                                                                \
            Register* src = GetRegister(modrm.reg);                                         \
            if (!src) ReturnFromBenchmark(false)                                            \
            const Expected<uint##size##_t> tmp = ReadPositioned<uint##size##_t>(addr);      \
            if (!tmp.HasValue()) ReturnFromBenchmark(false)                                 \
            a = tmp.Get();                                                                  \
            b = src->Get##size(false) + arg;                                                \
            result = a operation b;                                                         \
            if (!WritePositioned<uint##size##_t>(result, addr)) ReturnFromBenchmark(false); \
        })                                                                                  \
        UpdateFlags(result, a, b);                                                          \
        state.flags.carry = cf;                                                             \
        state.flags.overflow = of;                                                          \
        ReturnFromBenchmark(true)                                                           \
    }
    #define JumpIf(op, req, type)                                                       \
    case op: {                                                                          \
        const Expected<type> value = Fetch<type>();                                     \
        if (!value.HasValue()) ReturnFromBenchmark(false)                               \
        if (req) state.ip.Set16(state.ip.Get16(false) + value.Get(), false);            \
        ReturnFromBenchmark(true)                                                       \
    }
    // TODO: Modify C Z S O P A
    #define Neg(op, size)                                                               \
    case op: {                                                                          \
        const Expected<uint8_t> tmp = Fetch<uint8_t>();                                 \
        if (!tmp.HasValue()) ReturnFromBenchmark(false)                                 \
        const X86ModRM modrm = tmp.Get();                                               \
        ModRMOperation(switch (modrm.reg) {                                             \
            case 0b010: {                                                               \
                reg->Set##size(~reg->Get##size(false), false);                          \
                ReturnFromBenchmark(true)                                               \
            }                                                                           \
            case 0b011: {                                                               \
                reg->Set##size(-reg->Get##size(false), false);                          \
                ReturnFromBenchmark(true)                                               \
            }                                                                           \
            default: ReturnFromBenchmark(false)                                         \
        }, switch (modrm.reg) {                                                         \
            case 0b010: {                                                               \
                ReturnFromBenchmark(WritePositioned<uint##size##_t>(~tmp.Get(), addr)); \
            }                                                                           \
            case 0b011: {                                                               \
                ReturnFromBenchmark(WritePositioned<uint##size##_t>(-tmp.Get(), addr)); \
            }                                                                           \
            default: ReturnFromBenchmark(false)                                         \
        })                                                                              \
    }
    X86::X86(const Sequence<uint8_t>& memory, const X86State& state) : StepEmulator(memory), state(state) {
        EmptyBenchmark
    }
    void X86::UpdateFlags(uint64_t val, uint64_t a, uint64_t b) {
        StartBenchmark
        state.flags.zero = !val;
        state.flags.sign = val & 1 << 15;
        uint8_t parity = val & UINT8_MAX;
        parity ^= parity >> 4;
        parity ^= parity >> 2;
        parity ^= parity >> 1;
        state.flags.parity = !(parity & 1);
        state.flags.auxiliaryCarry = (a ^ b ^ val) & 1 << 4;
        EndBenchmark
    }
    Register* X86::GetRegister(uint8_t code) {
        StartBenchmark
        switch ((X86RegisterCode)code) {
            case X86RegisterCode::A: ReturnFromBenchmark(&state.a);
            case X86RegisterCode::C: ReturnFromBenchmark(&state.c);
            case X86RegisterCode::D: ReturnFromBenchmark(&state.d);
            case X86RegisterCode::B: ReturnFromBenchmark(&state.b);
            case X86RegisterCode::SP: ReturnFromBenchmark(&state.sp);
            case X86RegisterCode::BP: ReturnFromBenchmark(&state.bp);
            case X86RegisterCode::SI: ReturnFromBenchmark(&state.si);
            case X86RegisterCode::DI: ReturnFromBenchmark(&state.di);
            default: ReturnFromBenchmark(nullptr);
        }
    }
    String X86::ToString(const Sequence<char>& padding) const {
        StartAndReturnFromBenchmark(state.ToString(padding));
    }
    uint64_t X86::ToLinear(uint64_t segment, uint64_t offset) {
        StartAndReturnFromBenchmark(segment * 16 + offset);
    }
    void X86::Interrupt(uint8_t index) {
        (void)index;
        // TODO:
        EmptyBenchmark
    }
    Register X86::GetPC(void) const {
        StartAndReturnFromBenchmark(state.ip);
    }
    bool X86::Step(void) {
        StartBenchmark
        const Expected<uint8_t> opcode = Fetch<uint8_t>();
        if (!opcode.HasValue()) ReturnFromBenchmark(false);
        switch ((X86Opcode)opcode.Get()) {
            case X86Opcode::Hlt: {
                state.ip.value--;
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Int: {
                const Expected<uint8_t> tmp = Fetch<uint8_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                Interrupt(tmp.Get());
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Int3: {
                Interrupt(3);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Into: {
                if (state.flags.overflow) Interrupt(4);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Sahf: {
                Register tmp = state.flags.value;
                tmp.Set8(state.a.Get8(true), false);
                state.flags.value = tmp.value;
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Lahf: {
                state.a.Set8(state.flags.value, true);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Cbw: {
                state.a.Set8(state.a.Get8(false) & 1 << 7 ? UINT8_MAX : 0, true);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Cwd: {
                state.d.Set16(state.a.Get16(false) & 1 << 15 ? UINT16_MAX : 0, true);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Cmps8: {
                Expected<uint8_t> a = ReadPositioned<uint8_t>(ToLinear(state.ds.Get16(false), state.si.Get16(false)));
                if (!a.HasValue()) ReturnFromBenchmark(false)
                Expected<uint8_t> b = ReadPositioned<uint8_t>(ToLinear(state.es.Get16(false), state.di.Get16(false)));
                if (!b.HasValue()) ReturnFromBenchmark(false)
                UpdateFlags(a.Get() - b.Get(), a.Get(), b.Get());
                state.flags.carry = a.Get() < b.Get();
                state.flags.overflow = (a.Get() ^ b.Get()) & (a.Get() ^ (a.Get() - b.Get())) & 0x8000;
                if (state.flags.direction) {
                    state.si.Set16(state.si.Get16(false) + sizeof(uint8_t), false);
                    state.di.Set16(state.di.Get16(false) + sizeof(uint8_t), false);
                }
                else {
                    state.si.Set16(state.si.Get16(false) - sizeof(uint8_t), false);
                    state.di.Set16(state.di.Get16(false) - sizeof(uint8_t), false);
                }
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Cmps16: {
                Expected<uint16_t> a = ReadPositioned<uint16_t>(ToLinear(state.ds.Get16(false), state.si.Get16(false)));
                if (!a.HasValue()) ReturnFromBenchmark(false)
                Expected<uint16_t> b = ReadPositioned<uint16_t>(ToLinear(state.es.Get16(false), state.di.Get16(false)));
                if (!b.HasValue()) ReturnFromBenchmark(false)
                UpdateFlags(a.Get() - b.Get(), a.Get(), b.Get());
                state.flags.carry = a.Get() < b.Get();
                state.flags.overflow = (a.Get() ^ b.Get()) & (a.Get() ^ (a.Get() - b.Get())) & 0x8000;
                if (state.flags.direction) {
                    state.si.Set16(state.si.Get16(false) + sizeof(uint16_t), false);
                    state.di.Set16(state.di.Get16(false) + sizeof(uint16_t), false);
                }
                else {
                    state.si.Set16(state.si.Get16(false) - sizeof(uint16_t), false);
                    state.di.Set16(state.di.Get16(false) - sizeof(uint16_t), false);
                }
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Scas8: {
                Expected<uint8_t> a = ReadPositioned<uint8_t>(ToLinear(state.es.Get16(false), state.di.Get16(false)));
                if (!a.HasValue()) ReturnFromBenchmark(false)
                UpdateFlags(a.Get() - state.a.Get8(false), a.Get(), state.a.Get8(false));
                state.flags.carry = a.Get() < state.a.Get8(false);
                state.flags.overflow = (a.Get() ^ state.a.Get8(false)) & (a.Get() ^ (a.Get() - state.a.Get8(false))) & 0x8000;
                if (state.flags.direction) state.di.Set16(state.di.Get16(false) + sizeof(uint8_t), false);
                else state.di.Set16(state.di.Get16(false) - sizeof(uint8_t), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Scas16: {
                Expected<uint16_t> a = ReadPositioned<uint16_t>(ToLinear(state.es.Get16(false), state.di.Get16(false)));
                if (!a.HasValue()) ReturnFromBenchmark(false)
                UpdateFlags(a.Get() - state.a.Get16(false), a.Get(), state.a.Get16(false));
                state.flags.carry = a.Get() < state.a.Get16(false);
                state.flags.overflow = (a.Get() ^ state.a.Get16(false)) & (a.Get() ^ (a.Get() - state.a.Get16(false))) & 0x8000;
                if (state.flags.direction) state.di.Set16(state.di.Get16(false) + sizeof(uint16_t), false);
                else state.di.Set16(state.di.Get16(false) - sizeof(uint16_t), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Les: {
                const Expected<uint8_t> tmp = Fetch<uint8_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                const X86ModRM modrm = tmp.Get();
                ModRMOperation({
                    // TODO: Handle registers
                    ReturnFromBenchmark(false)
                }, {
                    Register* reg = GetRegister(modrm.reg);
                    uint16_t val;
                    if (!ReadPositioned<uint16_t>(val, addr)) ReturnFromBenchmark(false)
                    reg->Set16(val, false);
                    if (!ReadPositioned<uint16_t>(val, addr + sizeof(uint16_t))) ReturnFromBenchmark(false)
                    state.es.Set16(val, false);
                    ReturnFromBenchmark(true)
                })
            }
            case X86Opcode::Lds: {
                const Expected<uint8_t> tmp = Fetch<uint8_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                const X86ModRM modrm = tmp.Get();
                ModRMOperation({
                    // TODO: Handle registers
                    ReturnFromBenchmark(false)
                }, {
                    Register* reg = GetRegister(modrm.reg);
                    uint16_t val;
                    if (!ReadPositioned<uint16_t>(val, addr)) ReturnFromBenchmark(false)
                    reg->Set16(val, false);
                    if (!ReadPositioned<uint16_t>(val, addr + sizeof(uint16_t))) ReturnFromBenchmark(false)
                    state.ds.Set16(val, false);
                    ReturnFromBenchmark(true)
                })
            }
            case X86Opcode::Mov8Start ... X86Opcode::Mov8End: {
                uint8_t tmp = opcode.Get() - (uint8_t)X86Opcode::Mov8Start;
                bool upper = false;
                if (tmp > 3) {
                    tmp -= 4;
                    upper = true;
                }
                Register* reg = GetRegister(tmp);
                const Expected<uint8_t> val = Fetch<uint8_t>();
                if (!reg || !val.HasValue()) ReturnFromBenchmark(false)
                reg->Set8(val.Get(), upper);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Mov16Start ... X86Opcode::Mov16End: {
                Register* reg = GetRegister(opcode.Get() - (uint8_t)X86Opcode::Mov16Start);
                if (!reg) ReturnFromBenchmark(false)
                const Expected<uint16_t> val = Fetch<uint16_t>();
                if (!val.HasValue()) ReturnFromBenchmark(false)
                reg->Set16(val.Get(), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Xchg8: {
                const Expected<uint8_t> tmp = Fetch<uint8_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                const X86ModRM modrm = tmp.Get();
                ModRMOperation({
                    Register* src = GetRegister(modrm.reg);
                    const uint8_t tmp = reg->Get8(false);
                    reg->Set8(src->Get8(false), false);
                    src->Set8(tmp, false);
                    ReturnFromBenchmark(true)
                }, {
                    // TODO: Handle memory
                    ReturnFromBenchmark(false)
                })
            }
            case X86Opcode::XchgRegAddr16: {
                const Expected<uint8_t> tmp = Fetch<uint8_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                const X86ModRM modrm = tmp.Get();
                ModRMOperation({
                    Register* src = GetRegister(modrm.reg);
                    const uint8_t tmp = reg->Get8(false);
                    reg->Set8(src->Get8(false), false);
                    src->Set8(tmp, false);
                    ReturnFromBenchmark(true)
                }, {
                    Register* reg = GetRegister(modrm.reg);
                    uint16_t val;
                    if (!ReadPositioned<uint16_t>(val, addr)) ReturnFromBenchmark(false)
                    if (!WritePositioned<uint16_t>(reg->Get16(false), addr)) ReturnFromBenchmark(false)
                    reg->Set16(val, false);
                    ReturnFromBenchmark(true)
                })
            }
            case X86Opcode::Xchg16Start ... X86Opcode::Xchg16End: {
                Register* reg = GetRegister(opcode.Get() - (uint8_t)X86Opcode::Xchg16Start);
                if (!reg) ReturnFromBenchmark(false)
                const uint16_t tmp = state.a.Get16(false);
                state.a.Set16(reg->Get16(false), false);
                reg->Set16(tmp, false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Inc16Start ... X86Opcode::Inc16End: {
                Register* reg = GetRegister(opcode.Get() - (uint8_t)X86Opcode::Inc16Start);
                if (!reg) ReturnFromBenchmark(false)
                reg->value++;
                UpdateFlags(reg->value, reg->value - 1, 1);
                if (!reg->value) state.flags.overflow = true;
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Dec16Start ... X86Opcode::Dec16End: {
                Register* reg = GetRegister(opcode.Get() - (uint8_t)X86Opcode::Dec16Start);
                if (!reg) ReturnFromBenchmark(false)
                reg->value--;
                UpdateFlags(reg->value, reg->value + 1, 1);
                if (reg->value == UINT64_MAX) state.flags.overflow = true;
                ReturnFromBenchmark(true)
            }
            case X86Opcode::PushReg16Start ... X86Opcode::PushReg16End: {
                Register* reg = GetRegister(opcode.Get() - (uint8_t)X86Opcode::PushReg16Start);
                ReturnFromBenchmark(reg && Push<uint16_t>(reg->Get16(false)));
            }
            case X86Opcode::PopReg16Start ... X86Opcode::PopReg16End: {
                Register* reg = GetRegister(opcode.Get() - (uint8_t)X86Opcode::PopReg16Start);
                if (!reg) ReturnFromBenchmark(false)
                const Expected<uint16_t> tmp = Pop<uint16_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                reg->Set16(tmp.Get(), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::PushES16: ReturnFromBenchmark(Push<uint16_t>(state.es.Get16(false)))
            case X86Opcode::PushCS16: ReturnFromBenchmark(Push<uint16_t>(state.cs.Get16(false)))
            case X86Opcode::PushSS16: ReturnFromBenchmark(Push<uint16_t>(state.ss.Get16(false)))
            case X86Opcode::PushDS16: ReturnFromBenchmark(Push<uint16_t>(state.ds.Get16(false)))
            case X86Opcode::PopES16: {
                const Expected<uint16_t> tmp = Pop<uint16_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.es.Set16(tmp.Get(), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::PopSS16: {
                const Expected<uint16_t> tmp = Pop<uint16_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.ss.Set16(tmp.Get(), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::PopDS16: {
                const Expected<uint16_t> tmp = Pop<uint16_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.ds.Set16(tmp.Get(), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Pusha16: {
                const Register tmp = state.sp;
                ReturnFromBenchmark((
                    Push<uint16_t>(state.a.Get16(false)) &&
                    Push<uint16_t>(state.c.Get16(false)) &&
                    Push<uint16_t>(state.d.Get16(false)) &&
                    Push<uint16_t>(state.b.Get16(false)) &&
                    Push<uint16_t>(tmp.Get16(false)) &&
                    Push<uint16_t>(state.bp.Get16(false)) &&
                    Push<uint16_t>(state.si.Get16(false)) &&
                    Push<uint16_t>(state.di.Get16(false))
                ))
            }
            case X86Opcode::Popa16: {
                Expected<uint16_t> tmp = Pop<uint16_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.di.Set16(tmp.Get(), false);
                tmp = Pop<uint16_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.si.Set16(tmp.Get(), false);
                tmp = Pop<uint16_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.bp.Set16(tmp.Get(), false);
                if (!Pop<uint16_t>().HasValue()) ReturnFromBenchmark(false)
                tmp = Pop<uint16_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.b.Set16(tmp.Get(), false);
                tmp = Pop<uint16_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.d.Set16(tmp.Get(), false);
                tmp = Pop<uint16_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.c.Set16(tmp.Get(), false);
                tmp = Pop<uint16_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.a.Set16(tmp.Get(), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Pushf16: ReturnFromBenchmark(Push<uint16_t>(Register(state.flags.value).Get16(false)))
            case X86Opcode::Popf16: {
                const Expected<uint16_t> tmp = Pop<uint16_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                Register reg = state.flags.value;
                reg.Set16(tmp.Get(), false);
                state.flags.value = reg.value;
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Call16: {
                const Expected<uint16_t> val = Fetch<uint16_t>();
                if (!val.HasValue() || !Push<uint16_t>(state.ip.Get16(false))) ReturnFromBenchmark(false)
                state.ip.Set16(state.ip.Get16(false) + val.Get(), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::ImmediateRet16: {
                Expected<uint16_t> tmp = Pop<uint16_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.ip.Set16(tmp.Get(), false);
                tmp = Fetch<uint16_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.sp.Set16(state.sp.Get16(false) + tmp.Get(), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Ret16: {
                Expected<uint16_t> tmp = Pop<uint16_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.ip.Set16(tmp.Get(), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::ImmediateRetf16: {
                Expected<uint16_t> tmp = Pop<uint16_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.ip.Set16(tmp.Get(), false);
                tmp = Pop<uint16_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.cs.Set16(tmp.Get(), false);
                tmp = Fetch<uint16_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.sp.Set16(state.sp.Get16(false) + tmp.Get(), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Retf16: {
                Expected<uint16_t> tmp = Pop<uint16_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.ip.Set16(tmp.Get(), false);
                tmp = Pop<uint16_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.cs.Set16(tmp.Get(), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::IRet16: {
                uint16_t tmp[3];
                for (uint8_t i = 0; i < SizeOfArray(tmp); i++) {
                    const Expected<uint16_t> t = Pop<uint16_t>();
                    if (!t.HasValue()) ReturnFromBenchmark(false)
                    tmp[i] = t.Get();
                }
                state.ip.Set16(tmp[0], false);
                state.cs.Set16(tmp[1], false);
                Register reg = state.flags.value;
                reg.Set16(tmp[2], false);
                state.flags.value = reg.value;
                ReturnFromBenchmark(true)
            }
            JumpIf(X86Opcode::Jo8, state.flags.overflow, int8_t)
            JumpIf(X86Opcode::Jno8, !state.flags.overflow, int8_t)
            JumpIf(X86Opcode::Jc8, state.flags.carry, int8_t)
            JumpIf(X86Opcode::Jnc8, !state.flags.carry, int8_t)
            JumpIf(X86Opcode::Jz8, state.flags.zero, int8_t)
            JumpIf(X86Opcode::Jnz8, !state.flags.zero, int8_t)
            JumpIf(X86Opcode::Jbe8, state.flags.carry || state.flags.zero, int8_t)
            JumpIf(X86Opcode::Ja8, state.flags.carry && state.flags.zero, int8_t)
            JumpIf(X86Opcode::Js8, state.flags.sign, int8_t)
            JumpIf(X86Opcode::Jns8, !state.flags.sign, int8_t)
            JumpIf(X86Opcode::Jp8, state.flags.parity, int8_t)
            JumpIf(X86Opcode::Jnp8, !state.flags.parity, int8_t)
            JumpIf(X86Opcode::Jl8, state.flags.sign != state.flags.overflow, int8_t)
            JumpIf(X86Opcode::Jge8, state.flags.sign == state.flags.overflow, int8_t)
            JumpIf(X86Opcode::Jle8, state.flags.sign != state.flags.overflow || state.flags.zero, int8_t)
            JumpIf(X86Opcode::Jg8, !state.flags.zero && state.flags.sign == state.flags.overflow, int8_t)
            JumpIf(X86Opcode::Jcxz8, !state.c.Get16(false), int8_t)
            JumpIf(X86Opcode::Jmp16, true, uint16_t)
            JumpIf(X86Opcode::Jmp8, true, int8_t)
            case X86Opcode::Stos8: {
                if (!WritePositioned<uint8_t>(state.a.Get8(false), ToLinear(state.es.value, state.di.value))) ReturnFromBenchmark(false)
                if (state.flags.direction) state.di.Set16(state.di.Get16(false) - sizeof(uint8_t), false);
                else state.di.Set16(state.di.Get16(false) + sizeof(uint8_t), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Stos16: {
                if (!WritePositioned<uint16_t>(state.a.Get16(false), ToLinear(state.es.value, state.di.value))) ReturnFromBenchmark(false)
                if (state.flags.direction) state.di.Set16(state.di.Get16(false) - sizeof(uint16_t), false);
                else state.di.Set16(state.di.Get16(false) + sizeof(uint16_t), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Lods8: {
                const Expected<uint8_t> tmp = ReadPositioned<uint8_t>(ToLinear(state.ds.value, state.si.value));
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.a.Set8(tmp.Get(), false);
                if (state.flags.direction) state.si.Set16(state.si.Get16(false) - sizeof(uint8_t), false);
                else state.si.Set16(state.si.Get16(false) + sizeof(uint8_t), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Lods16: {
                const Expected<uint16_t> tmp = ReadPositioned<uint16_t>(ToLinear(state.ds.value, state.si.value));
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.a.Set16(tmp.Get(), false);
                if (state.flags.direction) state.si.Set16(state.si.Get16(false) - sizeof(uint16_t), false);
                else state.si.Set16(state.si.Get16(false) + sizeof(uint16_t), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Movs8: {
                const Expected<uint8_t> tmp = ReadPositioned<uint8_t>(ToLinear(state.ds.value, state.si.value));
                if (!tmp.HasValue() || !WritePositioned<uint8_t>(tmp.Get(), ToLinear(state.es.value, state.di.value))) ReturnFromBenchmark(false)
                if (state.flags.direction) {
                    state.si.Set16(state.si.Get16(false) - sizeof(uint8_t), false);
                    state.di.Set16(state.di.Get16(false) - sizeof(uint8_t), false);
                }
                else {
                    state.si.Set16(state.si.Get16(false) + sizeof(uint8_t), false);
                    state.di.Set16(state.di.Get16(false) + sizeof(uint8_t), false);
                }
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Movs16: {
                const Expected<uint16_t> tmp = ReadPositioned<uint16_t>(ToLinear(state.ds.value, state.si.value));
                if (!tmp.HasValue() || !WritePositioned<uint16_t>(tmp.Get(), ToLinear(state.es.value, state.di.value))) ReturnFromBenchmark(false)
                if (state.flags.direction) {
                    state.si.Set16(state.si.Get16(false) - sizeof(uint16_t), false);
                    state.di.Set16(state.di.Get16(false) - sizeof(uint16_t), false);
                }
                else {
                    state.si.Set16(state.si.Get16(false) + sizeof(uint16_t), false);
                    state.di.Set16(state.di.Get16(false) + sizeof(uint16_t), false);
                }
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Cmp16: {
                const Expected<uint8_t> tmp = Fetch<uint8_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                const X86ModRM modrm = tmp.Get();
                uint64_t a;
                uint64_t b;
                ModRMOperation({
                    Register* src = GetRegister(modrm.reg);
                    if (!src) ReturnFromBenchmark(false)
                    a = reg->value;
                    b = src->value;
                }, {
                    // TODO: Handle memory
                    ReturnFromBenchmark(false)
                })
                UpdateFlags(a - b, a, b);
                state.flags.carry = a < b;
                state.flags.overflow = (a ^ b) & (a ^ (a - b)) & 0x8000;
                ReturnFromBenchmark(true)
            }
            Neg(X86Opcode::Neg8, 8)
            Neg(X86Opcode::Neg16, 16)
            case X86Opcode::Cmc: {
                state.flags.carry = !state.flags.carry;
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Clc: {
                state.flags.carry = false;
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Stc: {
                state.flags.carry = true;
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Cli: {
                state.flags.interruptEnable = false;
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Sti: {
                state.flags.interruptEnable = true;
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Cld: {
                state.flags.direction = false;
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Std: {
                state.flags.direction = true;
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Shr16: {
                const Expected<uint8_t> tmp = Fetch<uint8_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                const X86ModRM modrm = tmp.Get();
                uint64_t a;
                uint8_t b = state.c.Get8(false);
                uint64_t result;
                ModRMOperation({
                    a = reg->value;
                    if (modrm.reg == 0b101) reg->value >>= b;
                    else if (modrm.reg == 0b100) reg->value <<= b;
                    else ReturnFromBenchmark(false)
                    result = reg->value;
                }, {
                    // TODO: Handle memory
                    ReturnFromBenchmark(false)
                })
                UpdateFlags(result, a, b);
                if (modrm.reg == 0b101) state.flags.carry = (a >> (b - 1)) & 0x1;
                else if (modrm.reg == 0b100) state.flags.carry = (a >> (16 - b)) & 0x1;
                else ReturnFromBenchmark(false)
                if (b == 1) state.flags.overflow = ((result >> 15) & 0x1) ^ ((result >> 14) & 0x1);
                ReturnFromBenchmark(true)
            }
            AnyRegMathOperation(X86Opcode::AddAnyReg8, +, 8, result > UINT16_MAX, (a ^ result) & (b ^ result) & 0x8000, 0)
            AnyRegMathOperation(X86Opcode::AddAnyReg16, +, 16, result > UINT16_MAX, (a ^ result) & (b ^ result) & 0x8000, 0)
            AnyRegMathOperation(X86Opcode::AdcAnyReg8, +, 8, result > UINT16_MAX, (a ^ result) & (b ^ result) & 0x8000, state.flags.carry)
            AnyRegMathOperation(X86Opcode::AdcAnyReg16, +, 16, result > UINT16_MAX, (a ^ result) & (b ^ result) & 0x8000, state.flags.carry)
            AnyRegMathOperation(X86Opcode::SubAnyReg8, -, 8, a < b, (a ^ b) & (a ^ result) & 0x8000, 0)
            AnyRegMathOperation(X86Opcode::SubAnyReg16, -, 16, a < b, (a ^ b) & (a ^ result) & 0x8000, 0)
            AnyRegMathOperation(X86Opcode::SbbAnyReg8, -, 8, a < b, (a ^ b) & (a ^ result) & 0x8000, state.flags.carry)
            AnyRegMathOperation(X86Opcode::SbbAnyReg16, -, 16, a < b, (a ^ b) & (a ^ result) & 0x8000, state.flags.carry)
            AnyRegMathOperation(X86Opcode::AndAnyReg8, &, 8, false, false, 0)
            AnyRegMathOperation(X86Opcode::AndAnyReg16, &, 16, false, false, 0)
            AnyRegMathOperation(X86Opcode::OrAnyReg8, |, 8, false, false, 0)
            AnyRegMathOperation(X86Opcode::OrAnyReg16, |, 16, false, false, 0)
            AnyRegMathOperation(X86Opcode::XorAnyReg8, ^, 8, false, false, 0)
            AnyRegMathOperation(X86Opcode::XorAnyReg16, ^, 16, false, false, 0)
            case X86Opcode::Aaa: {
                if (state.a.Get4(false) > 9 || state.flags.auxiliaryCarry) {
                    state.a.Set8(state.a.Get8(false) + 6, false);
                    state.a.Set8(state.a.Get8(true) + 1, true);
                    state.flags.auxiliaryCarry = state.flags.carry = true;
                }
                else state.flags.auxiliaryCarry = state.flags.carry = false;
                state.a.Clear4(true);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Aas: {
                if (state.a.Get4(false) > 9 || state.flags.auxiliaryCarry) {
                    state.a.Set8(state.a.Get8(false) - 6, false);
                    state.a.Set8(state.a.Get8(true) - 1, true);
                    state.flags.auxiliaryCarry = state.flags.carry = true;
                }
                else state.flags.auxiliaryCarry = state.flags.carry = false;
                state.a.Clear4(true);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Aam: {
                const Expected<uint8_t> tmp = Fetch<uint8_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                // TODO: Set Z, S and P
                state.a.Set8(state.a.Get8(false) / tmp.Get(), true);
                state.a.Set8(state.a.Get8(false) % tmp.Get(), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Aad: {
                const Expected<uint8_t> tmp = Fetch<uint8_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                // TODO: Set Z, S and P
                state.a.Set8(state.a.Get8(true) * tmp.Get() + state.a.Get8(false), false);
                state.a.Set8(0, true);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Daa: {
                // TODO: Set Z, S, O, P
                if (state.a.Get4(false) > 9 || state.flags.auxiliaryCarry) {
                    state.a.Set8(state.a.Get8(false) + 6, false);
                    state.flags.auxiliaryCarry = true;
                }
                if (state.a.Get8(false) > 0x9f || state.flags.carry) {
                    state.a.Set8(state.a.Get8(false) + 0x60, false);
                    state.flags.carry = true;
                }
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Das: {
                // TODO: Set Z, S, O, P
                if (state.a.Get4(false) > 9 || state.flags.auxiliaryCarry) {
                    state.a.Set8(state.a.Get8(false) - 6, false);
                    state.flags.auxiliaryCarry = true;
                }
                if (state.a.Get8(false) > 0x9f || state.flags.carry) {
                    state.a.Set8(state.a.Get8(false) - 0x60, false);
                    state.flags.carry = true;
                }
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Xlat8: {
                state.a.Set8(ToLinear(state.ds.value, state.b.value + state.a.Get8(false)), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Loop: {
                const Expected<int8_t> tmp = Fetch<int8_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.c.Set16(state.c.Get16(false) - 1, false);
                if (state.c.Get16(false))
                    state.ip.Set16(state.ip.Get16(false) + tmp.Get(), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Loopz: {
                const Expected<int8_t> tmp = Fetch<int8_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.c.Set16(state.c.Get16(false) - 1, false);
                if (state.flags.zero && state.c.Get16(false)) state.ip.Set16(state.ip.Get16(false) + tmp.Get(), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Loopnz: {
                const Expected<uint8_t> tmp = Fetch<uint8_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false)
                state.c.Set16(state.c.Get16(false) - 1, false);
                if (!state.flags.zero && state.c.Get16(false)) state.ip.Set16(state.ip.Get16(false) + tmp.Get(), false);
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Rep: {
                const size_t ip = state.ip.Get16(false);
                bool repz = false;
                const Expected<uint8_t> opcode = Fetch<uint8_t>();
                if (!opcode.HasValue()) ReturnFromBenchmark(false)
                switch ((X86Opcode)opcode.Get()) {
                    // TODO: Cmps32 Scas32
                    case X86Opcode::Cmps8:
                    case X86Opcode::Cmps16:
                    case X86Opcode::Scas8:
                    case X86Opcode::Scas16: {
                        repz = true;
                        break;
                    }
                    default: break;
                }
                while (true) {
                    state.ip.Set16(ip, false);
                    if (!Step()) ReturnFromBenchmark(false)
                    state.c.Set16(state.c.Get16(false) - 1, false);
                    if (!state.c.Get16(false) || (repz && !state.flags.zero)) break;
                }
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Repnz: {
                const size_t ip = state.ip.Get16(false);
                while (true) {
                    if (!Step()) ReturnFromBenchmark(false)
                    state.c.Set16(state.c.Get16(false) - 1, false);
                    if (!state.c.Get16(false) || state.flags.zero) break;
                    else state.ip.Set16(ip, false);
                }
                ReturnFromBenchmark(true)
            }
            case X86Opcode::Extended: {
                JumpIf(X86Opcode::ExtendedJc16, state.flags.carry, uint16_t)
                JumpIf(X86Opcode::ExtendedJnc16, !state.flags.carry, uint16_t)
                JumpIf(X86Opcode::ExtendedJz16, state.flags.zero, uint16_t)
                JumpIf(X86Opcode::ExtendedJnz16, !state.flags.zero, uint16_t)
            }
            default: ReturnFromBenchmark(false)
        }
    }
}