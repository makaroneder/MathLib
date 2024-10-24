#include "X86.hpp"
#include "X86ModRM.hpp"

#define MathOperation(op, operation, cf, of, arg)                                   \
case op: {                                                                          \
    const Expected<uint8_t> tmp = Fetch<uint8_t>();                                 \
    if (!tmp.HasValue()) return false;                                              \
    const X86ModRM modrm = tmp.Get();                                               \
    uint64_t result;                                                                \
    uint64_t a;                                                                     \
    uint64_t b;                                                                     \
    switch (modrm.mode) {                                                           \
        case 0b11: {                                                                \
            Register* src = GetRegister(modrm.src);                                 \
            if (!src) return false;                                                 \
            Register* dst = GetRegister(modrm.dst);                                 \
            if (!dst) return false;                                                 \
            a = dst->value;                                                         \
            b = src->value + arg;                                                   \
            dst->value operation##= b;                                              \
            result = dst->value;                                                    \
            break;                                                                  \
        }                                                                           \
        default: return false;                                                      \
    }                                                                               \
    UpdateFlags(result, a, b);                                                      \
    state.flags.carry = cf;                                                         \
    state.flags.overflow = of;                                                      \
    return true;                                                                    \
}
#define JumpIf(op, req, size)                                                       \
case op: {                                                                          \
    const Expected<uint##size##_t> value = Fetch<uint##size##_t>();                 \
    if (!value.HasValue()) return false;                                            \
    if (req) state.ip.Set##size(state.ip.Get##size(false) + value.Get(), false);    \
    return true;                                                                    \
}
X86::X86(const Array<uint8_t>& mem, const X86State& state) : Emulator(mem), state(state) {}
void X86::UpdateFlags(uint64_t val, uint64_t a, uint64_t b) {
    state.flags.zero = !val;
    state.flags.sign = val & 1 << 15;
    uint8_t parity = val & UINT8_MAX;
    parity ^= parity >> 4;
    parity ^= parity >> 2;
    parity ^= parity >> 1;
    state.flags.parity = !(parity & 1);
    state.flags.auxiliaryCarry = (a ^ b ^ val) & 1 << 4;
}
Register* X86::GetRegister(uint8_t code) {
    switch ((X86RegisterCode)code) {
        case X86RegisterCode::A: return &state.a;
        case X86RegisterCode::C: return &state.c;
        case X86RegisterCode::D: return &state.d;
        case X86RegisterCode::B: return &state.b;
        case X86RegisterCode::SP: return &state.sp;
        case X86RegisterCode::BP: return &state.bp;
        case X86RegisterCode::SI: return &state.si;
        case X86RegisterCode::DI: return &state.di;
        default: return nullptr;
    }
}
String X86::ToString(const String& padding) const {
    return state.ToString(padding);
}
bool X86::Run(void) {
    while (state.ip.value < GetSize())
        if (!Step()) return false;
    return true;
}
uint64_t X86::ToLinear(uint64_t segment, uint64_t offset) {
    return segment * 16 + offset;
}
bool X86::Step(void) {
    const Expected<uint8_t> opcode = Fetch<uint8_t>();
    if (!opcode.HasValue()) return false;
    switch ((X86Opcode)opcode.Get()) {
        case X86Opcode::Nop: return true;
        case X86Opcode::Hlt: {
            state.ip.value--;
            return true;
        }
        case X86Opcode::Sahf: {
            Register tmp = state.flags.value;
            tmp.Set8(state.a.Get8(true), false);
            state.flags.value = tmp.value;
            return true;
        }
        case X86Opcode::Lahf: {
            state.a.Set8(state.flags.value, true);
            return true;
        }
        case X86Opcode::Cbw: {
            state.a.Set8(state.a.Get8(false) & 1 << 7 ? UINT8_MAX : 0, true);
            return true;
        }
        case X86Opcode::Cwd: {
            state.d.Set16(state.a.Get16(false) & 1 << 15 ? UINT16_MAX : 0, true);
            return true;
        }
        case X86Opcode::Mov8Start ... X86Opcode::Mov8End: {
            uint8_t tmp = opcode.Get() - 0xb0;
            bool upper = false;
            if (tmp > 3) {
                tmp -= 4;
                upper = true;
            }
            Register* reg = GetRegister(tmp);
            const Expected<uint8_t> val = Fetch<uint8_t>();
            if (!reg || !val.HasValue()) return false;
            reg->Set8(val.Get(), upper);
            return true;
        }
        case X86Opcode::Mov16Start ... X86Opcode::Mov16End: {
            Register* reg = GetRegister(opcode.Get() - 0xb8);
            if (!reg) return false;
            const Expected<uint16_t> val = Fetch<uint16_t>();
            if (!val.HasValue()) return false;
            reg->Set16(val.Get(), false);
            return true;
        }
        case X86Opcode::Inc16Start ... X86Opcode::Inc16End: {
            Register* reg = GetRegister(opcode.Get() - 0x40);
            if (!reg) return false;
            reg->value++;
            UpdateFlags(reg->value, reg->value - 1, 1);
            if (!reg->value) state.flags.overflow = true;
            return true;
        }
        case X86Opcode::Dec16Start ... X86Opcode::Dec16End: {
            Register* reg = GetRegister(opcode.Get() - 0x48);
            if (!reg) return false;
            reg->value--;
            UpdateFlags(reg->value, reg->value + 1, 1);
            if (reg->value == UINT64_MAX) state.flags.overflow = true;
            return true;
        }
        case X86Opcode::Push16Start ... X86Opcode::Push16End: {
            Register* reg = GetRegister(opcode.Get() - 0x50);
            return reg && Push<uint16_t>(reg->Get16(false));
        }
        case X86Opcode::Pop16Start ... X86Opcode::Pop16End: {
            Register* reg = GetRegister(opcode.Get() - 0x58);
            if (!reg) return false;
            const Expected<uint16_t> tmp = Pop<uint16_t>();
            if (!tmp.HasValue()) return false;
            reg->Set16(tmp.Get(), false);
            return true;
        }
        case X86Opcode::Pusha16: {
            const Register tmp = state.sp;
            return (
                Push<uint16_t>(state.a.Get16(false)) &&
                Push<uint16_t>(state.c.Get16(false)) &&
                Push<uint16_t>(state.d.Get16(false)) &&
                Push<uint16_t>(state.b.Get16(false)) &&
                Push<uint16_t>(tmp.Get16(false)) &&
                Push<uint16_t>(state.bp.Get16(false)) &&
                Push<uint16_t>(state.si.Get16(false)) &&
                Push<uint16_t>(state.di.Get16(false))
            );
        }
        case X86Opcode::Popa16: {
            Expected<uint16_t> tmp = Pop<uint16_t>();
            if (!tmp.HasValue()) return false;
            state.di.Set16(tmp.Get(), false);
            tmp = Pop<uint16_t>();
            if (!tmp.HasValue()) return false;
            state.si.Set16(tmp.Get(), false);
            tmp = Pop<uint16_t>();
            if (!tmp.HasValue()) return false;
            state.bp.Set16(tmp.Get(), false);
            if (!Pop<uint16_t>().HasValue()) return false;
            tmp = Pop<uint16_t>();
            if (!tmp.HasValue()) return false;
            state.b.Set16(tmp.Get(), false);
            tmp = Pop<uint16_t>();
            if (!tmp.HasValue()) return false;
            state.d.Set16(tmp.Get(), false);
            tmp = Pop<uint16_t>();
            if (!tmp.HasValue()) return false;
            state.c.Set16(tmp.Get(), false);
            tmp = Pop<uint16_t>();
            if (!tmp.HasValue()) return false;
            state.a.Set16(tmp.Get(), false);
            return true;
        }
        case X86Opcode::Pushf16: return Push<uint16_t>(Register(state.flags.value).Get16(false));
        case X86Opcode::Popf16: {
            const Expected<uint16_t> tmp = Pop<uint16_t>();
            if (!tmp.HasValue()) return false;
            Register reg = state.flags.value;
            reg.Set16(tmp.Get(), false);
            state.flags.value = reg.value;
            return true;
        }
        case X86Opcode::Call16: {
            const Expected<uint16_t> val = Fetch<uint16_t>();
            if (!val.HasValue() || !Push<uint16_t>(state.ip.Get16(false))) return false;
            state.ip.Set16(state.ip.Get16(false) + val.Get(), false);
            return true;
        }
        // TODO: Immediate ret16, iret16
        case X86Opcode::Ret16: {
            Expected<uint16_t> tmp = Pop<uint16_t>();
            if (!tmp.HasValue()) return false;
            state.ip.Set16(tmp.Get(), false);
            return true;
        }
        case X86Opcode::ImmediateRetf16: {
            Expected<uint16_t> tmp = Pop<uint16_t>();
            if (!tmp.HasValue()) return false;
            state.ip.Set16(tmp.Get(), false);
            tmp = Pop<uint16_t>();
            if (!tmp.HasValue()) return false;
            state.cs.Set16(tmp.Get(), false);
            tmp = Fetch<uint16_t>();
            if (!tmp.HasValue()) return false;
            state.sp.Set16(state.sp.Get16(false) + tmp.Get(), false);
            return true;
        }
        case X86Opcode::Retf16: {
            Expected<uint16_t> tmp = Pop<uint16_t>();
            if (!tmp.HasValue()) return false;
            state.ip.Set16(tmp.Get(), false);
            tmp = Pop<uint16_t>();
            if (!tmp.HasValue()) return false;
            state.cs.Set16(tmp.Get(), false);
            return true;
        }
        case X86Opcode::IRet16: {
            uint16_t tmp[3];
            for (uint8_t i = 0; i < SizeOfArray(tmp); i++) {
                const Expected<uint16_t> t = Pop<uint16_t>();
                if (!t.HasValue()) return false;
                tmp[i] = t.Get();
            }
            state.ip.Set16(tmp[0], false);
            state.cs.Set16(tmp[1], false);
            Register reg = state.flags.value;
            reg.Set16(tmp[2], false);
            state.flags.value = reg.value;
            return true;
        }
        JumpIf(X86Opcode::Jo8, state.flags.overflow, 8)
        JumpIf(X86Opcode::Jno8, !state.flags.overflow, 8)
        JumpIf(X86Opcode::Jc8, state.flags.carry, 8)
        JumpIf(X86Opcode::Jnc8, !state.flags.carry, 8)
        JumpIf(X86Opcode::Jz8, state.flags.zero, 8)
        JumpIf(X86Opcode::Jnz8, !state.flags.zero, 8)
        JumpIf(X86Opcode::Jbe8, state.flags.carry || state.flags.zero, 8)
        JumpIf(X86Opcode::Ja8, state.flags.carry && state.flags.zero, 8)
        JumpIf(X86Opcode::Js8, state.flags.sign, 8)
        JumpIf(X86Opcode::Jns8, !state.flags.sign, 8)
        JumpIf(X86Opcode::Jp8, state.flags.parity, 8)
        JumpIf(X86Opcode::Jnp8, !state.flags.parity, 8)
        JumpIf(X86Opcode::Jl8, state.flags.sign != state.flags.overflow, 8)
        JumpIf(X86Opcode::Jge8, state.flags.sign == state.flags.overflow, 8)
        JumpIf(X86Opcode::Jle8, state.flags.sign != state.flags.overflow || state.flags.zero, 8)
        JumpIf(X86Opcode::Jg8, !state.flags.zero && state.flags.sign == state.flags.overflow, 8)
        JumpIf(X86Opcode::Jcxz8, !state.c.Get16(false), 8)
        JumpIf(X86Opcode::Jmp16, true, 16)
        JumpIf(X86Opcode::Jmp8, true, 8)
        case X86Opcode::Stos8: {
            if (!WritePositioned<uint8_t>(state.a.Get8(false), ToLinear(state.es.value, state.di.value))) return false;
            if (state.flags.direction) state.di.value--;
            else state.di.value++;
            return true;
        }
        case X86Opcode::Lods8: {
            const Expected<uint8_t> tmp = ReadPositioned<uint8_t>(ToLinear(state.ds.value, state.si.value));
            if (!tmp.HasValue()) return false;
            state.a.Set8(tmp.Get(), false);
            if (state.flags.direction) state.si.value--;
            else state.si.value++;
            return true;
        }
        case X86Opcode::Movs8: {
            const Expected<uint8_t> tmp = ReadPositioned<uint8_t>(ToLinear(state.ds.value, state.si.value));
            if (!tmp.HasValue() || !WritePositioned<uint8_t>(tmp.Get(), ToLinear(state.es.value, state.di.value))) return false;
            if (state.flags.direction) {
                state.si.value--;
                state.di.value--;
            }
            else {
                state.si.value++;
                state.di.value++;
            }
            return true;
        }
        case X86Opcode::Cmp16: {
            const Expected<uint8_t> tmp = Fetch<uint8_t>();
            if (!tmp.HasValue()) return false;
            const X86ModRM modrm = tmp.Get();
            uint64_t a;
            uint64_t b;
            switch (modrm.mode) {
                case 0b11: {
                    Register* src = GetRegister(modrm.src);
                    if (!src) return false;
                    Register* dst = GetRegister(modrm.dst);
                    if (!dst) return false;
                    a = dst->value;
                    b = src->value;
                    break;
                }
                default: return false;
            }
            UpdateFlags(a - b, a, b);
            state.flags.carry = a < b;
            state.flags.overflow = (a ^ b) & (a ^ (a - b)) & 0x8000;
            return true;
        }
        case X86Opcode::Cmc: {
            state.flags.carry = !state.flags.carry;
            return true;
        }
        case X86Opcode::Clc: {
            state.flags.carry = false;
            return true;
        }
        case X86Opcode::Stc: {
            state.flags.carry = true;
            return true;
        }
        case X86Opcode::Cli: {
            state.flags.interruptEnable = false;
            return true;
        }
        case X86Opcode::Sti: {
            state.flags.interruptEnable = true;
            return true;
        }
        case X86Opcode::Cld: {
            state.flags.direction = false;
            return true;
        }
        case X86Opcode::Std: {
            state.flags.direction = true;
            return true;
        }
        case X86Opcode::Shr16: {
            const Expected<uint8_t> tmp = Fetch<uint8_t>();
            if (!tmp.HasValue()) return false;
            const X86ModRM modrm = tmp.Get();
            if (modrm.src != 0b101) return false;
            uint64_t a;
            uint8_t b = state.c.Get8(false);
            uint64_t result;
            switch (modrm.mode) {
                case 0b11: {
                    Register* dst = GetRegister(modrm.dst);
                    if (!dst) return false;
                    a = dst->value;
                    if (modrm.src == 0b101) dst->value >>= b;
                    else if (modrm.src == 0b100) dst->value <<= b;
                    else return false;
                    result = dst->value;
                    break;
                }
                default: return false;
            }
            UpdateFlags(result, a, b);
            if (modrm.src == 0b101) state.flags.carry = (a >> (b - 1)) & 0x1;
            else if (modrm.src == 0b100) state.flags.carry = (a >> (16 - b)) & 0x1;
            else return false;
            if (b == 1) state.flags.overflow = ((result >> 15) & 0x1) ^ ((result >> 14) & 0x1);
            return true;
        }
        MathOperation(X86Opcode::Or16, |, false, false, 0)
        MathOperation(X86Opcode::And16, &, false, false, 0)
        MathOperation(X86Opcode::Xor16, ^, false, false, 0)
        MathOperation(X86Opcode::Add16, +, result > UINT16_MAX, (a ^ result) & (b ^ result) & 0x8000, 0)
        MathOperation(X86Opcode::Adc16, +, result > UINT16_MAX, (a ^ result) & (b ^ result) & 0x8000, state.flags.carry)
        MathOperation(X86Opcode::Sub16, -, a < b, (a ^ b) & (a ^ result) & 0x8000, 0)
        MathOperation(X86Opcode::Sbb16, -, a < b, (a ^ b) & (a ^ result) & 0x8000, state.flags.carry)
        case X86Opcode::Aaa: {
            if (state.a.Get4(false) > 9 || state.flags.auxiliaryCarry) {
                state.a.Set8(state.a.Get8(false) + 6, false);
                state.a.Set8(state.a.Get8(true) + 1, true);
                state.flags.auxiliaryCarry = state.flags.carry = true;
            }
            else state.flags.auxiliaryCarry = state.flags.carry = false;
            state.a.Clear4(true);
            return true;
        }
        case X86Opcode::Aas: {
            if (state.a.Get4(false) > 9 || state.flags.auxiliaryCarry) {
                state.a.Set8(state.a.Get8(false) - 6, false);
                state.a.Set8(state.a.Get8(true) - 1, true);
                state.flags.auxiliaryCarry = state.flags.carry = true;
            }
            else state.flags.auxiliaryCarry = state.flags.carry = false;
            state.a.Clear4(true);
            return true;
        }
        case X86Opcode::Aam: {
            const Expected<uint8_t> tmp = Fetch<uint8_t>();
            if (!tmp.HasValue()) return false;
            // TODO: Set Z, S and P
            state.a.Set8(state.a.Get8(false) / tmp.Get(), true);
            state.a.Set8(state.a.Get8(false) % tmp.Get(), false);
            return true;
        }
        case X86Opcode::Aad: {
            const Expected<uint8_t> tmp = Fetch<uint8_t>();
            if (!tmp.HasValue()) return false;
            // TODO: Set Z, S and P
            state.a.Set8(state.a.Get8(true) * tmp.Get() + state.a.Get8(false), false);
            state.a.Set8(0, true);
            return true;
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
            return true;
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
            return true;
        }
        case X86Opcode::Xlat8: {
            state.a.Set8(ToLinear(state.ds.value, state.b.value + state.a.Get8(false)), false);
            return true;
        }
        case X86Opcode::Extended: {
            JumpIf(X86Opcode::ExtendedJc16, state.flags.carry, 16)
            JumpIf(X86Opcode::ExtendedJnc16, !state.flags.carry, 16)
            JumpIf(X86Opcode::ExtendedJz16, state.flags.zero, 16)
            JumpIf(X86Opcode::ExtendedJnz16, !state.flags.zero, 16)
        }
        default: return false;
    }
}