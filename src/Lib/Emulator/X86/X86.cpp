#include "X86.hpp"
#include "ModRM.hpp"

// TODO: xlatb daa das cmps scas test xchg div idiv imul lds lea les loop loope loopne loopnz
// TODO: loopz mul neg not rcl rcr rol ror sal sar rep repe repne repnz repz in out int into

#define MathOperation(op, operation, cf, of, arg)                                   \
case op: {                                                                          \
    const Expected<uint8_t> tmp = Fetch<uint8_t>();                                 \
    if (!tmp.HasValue()) return false;                                              \
    const ModRM modrm = tmp.Get();                                                  \
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
Register* X86::GetRegister(const uint8_t& code) {
    switch (code) {
        case 0: return &state.a;
        case 1: return &state.c;
        case 2: return &state.d;
        case 3: return &state.b;
        case 4: return &state.sp;
        case 5: return &state.bp;
        case 6: return &state.si;
        case 7: return &state.di;
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
    switch (opcode.Get()) {
        // nop
        case 0x90: return true;
        // hlt
        case 0xf4: {
            state.ip.value--;
            return true;
        }
        // sahf
        case 0x9e: {
            Register tmp = state.flags.value;
            tmp.Set8(state.a.Get8(true), false);
            state.flags.value = tmp.value;
            return true;
        }
        // lahf
        case 0x9f: {
            state.a.Set8(state.flags.value, true);
            return true;
        }
        // cbw
        case 0x98: {
            state.a.Set8(state.a.Get8(false) & 1 << 7 ? UINT8_MAX : 0, true);
            return true;
        }
        // cwd
        case 0x99: {
            state.d.Set16(state.a.Get16(false) & 1 << 15 ? UINT16_MAX : 0, true);
            return true;
        }
        // mov8
        case 0xb0 ... 0xb7: {
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
        // mov16
        case 0xb8 ... 0xbf: {
            Register* reg = GetRegister(opcode.Get() - 0xb8);
            if (!reg) return false;
            const Expected<uint16_t> val = Fetch<uint16_t>();
            if (!val.HasValue()) return false;
            reg->Set16(val.Get(), false);
            return true;
        }
        // inc
        case 0x40 ... 0x47: {
            Register* reg = GetRegister(opcode.Get() - 0x40);
            if (!reg) return false;
            reg->value++;
            UpdateFlags(reg->value, reg->value - 1, 1);
            if (!reg->value) state.flags.overflow = true;
            return true;
        }
        // dec
        case 0x48 ... 0x4f: {
            Register* reg = GetRegister(opcode.Get() - 0x48);
            if (!reg) return false;
            reg->value--;
            UpdateFlags(reg->value, reg->value + 1, 1);
            if (reg->value == UINT64_MAX) state.flags.overflow = true;
            return true;
        }
        // push16
        case 0x50 ... 0x57: {
            Register* reg = GetRegister(opcode.Get() - 0x50);
            return reg && Push<uint16_t>(reg->Get16(false));
        }
        // pop16
        case 0x58 ... 0x5f: {
            Register* reg = GetRegister(opcode.Get() - 0x58);
            if (!reg) return false;
            const Expected<uint16_t> tmp = Pop<uint16_t>();
            if (!tmp.HasValue()) return false;
            reg->Set16(tmp.Get(), false);
            return true;
        }
        // pusha16
        case 0x60: {
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
        // popa16
        case 0x61: {
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
        // pushf16
        case 0x9c: return Push<uint16_t>(Register(state.flags.value).Get16(false));
        // popf16
        case 0x9d: {
            const Expected<uint16_t> tmp = Pop<uint16_t>();
            if (!tmp.HasValue()) return false;
            Register reg = state.flags.value;
            reg.Set16(tmp.Get(), false);
            state.flags.value = reg.value;
            return true;
        }
        // call16
        case 0xe8: {
            const Expected<uint16_t> val = Fetch<uint16_t>();
            if (!val.HasValue() || !Push<uint16_t>(state.ip.Get16(false))) return false;
            state.ip.Set16(state.ip.Get16(false) + val.Get(), false);
            return true;
        }
        // TODO: Immediate ret16, iret16
        // ret16
        case 0xc3: {
            Expected<uint16_t> tmp = Pop<uint16_t>();
            if (!tmp.HasValue()) return false;
            state.ip.Set16(tmp.Get(), false);
            return true;
        }
        // immediate retf16
        case 0xca: {
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
        // retf16
        case 0xcb: {
            Expected<uint16_t> tmp = Pop<uint16_t>();
            if (!tmp.HasValue()) return false;
            state.ip.Set16(tmp.Get(), false);
            tmp = Pop<uint16_t>();
            if (!tmp.HasValue()) return false;
            state.cs.Set16(tmp.Get(), false);
            return true;
        }
        // iret16
        case 0xcf: {
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
        // jo8
        JumpIf(0x70, state.flags.overflow, 8)
        // jno8
        JumpIf(0x71, !state.flags.overflow, 8)
        // jc8 / jb8 / jnae8
        JumpIf(0x72, state.flags.carry, 8)
        // jnc8 / jnb8 / jae8
        JumpIf(0x73, !state.flags.carry, 8)
        // jz8 / je8
        JumpIf(0x74, state.flags.zero, 8)
        // jnz8 / jne8
        JumpIf(0x75, !state.flags.zero, 8)
        // jbe8 / jna8
        JumpIf(0x76, state.flags.carry || state.flags.zero, 8)
        // ja8 / jnbe8
        JumpIf(0x77, state.flags.carry && state.flags.zero, 8)
        // js8
        JumpIf(0x78, state.flags.sign, 8)
        // jns8
        JumpIf(0x79, !state.flags.sign, 8)
        // jp8 / jpe8
        JumpIf(0x7a, state.flags.parity, 8)
        // jnp8 / jpo8
        JumpIf(0x7b, !state.flags.parity, 8)
        // jl8 / jnge8
        JumpIf(0x7c, state.flags.sign != state.flags.overflow, 8)
        // jge8 / jnl8
        JumpIf(0x7d, state.flags.sign == state.flags.overflow, 8)
        // jle8 / jng8
        JumpIf(0x7e, state.flags.sign != state.flags.overflow || state.flags.zero, 8)
        // jg8 / jnle8
        JumpIf(0x7f, !state.flags.zero && state.flags.sign == state.flags.overflow, 8)
        // jcxz8
        JumpIf(0xe3, !state.c.Get16(false), 8)
        // jmp16
        JumpIf(0xe9, true, 16)
        // jmp8
        JumpIf(0xeb, true, 8)

        // stos8
        case 0xaa: {
            if (!WritePositioned<uint8_t>(state.a.Get8(false), ToLinear(state.es.value, state.di.value))) return false;
            if (state.flags.direction) state.di.value--;
            else state.di.value++;
            return true;
        }
        // lods8
        case 0xac: {
            const Expected<uint8_t> tmp = ReadPositioned<uint8_t>(ToLinear(state.ds.value, state.si.value));
            if (!tmp.HasValue()) return false;
            state.a.Set8(tmp.Get(), false);
            if (state.flags.direction) state.si.value--;
            else state.si.value++;
            return true;
        }
        // movs8
        case 0xa4: {
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
        // cmp16
        case 0x39: {
            const Expected<uint8_t> tmp = Fetch<uint8_t>();
            if (!tmp.HasValue()) return false;
            const ModRM modrm = tmp.Get();
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
        // cmc
        case 0xf5: {
            state.flags.carry = !state.flags.carry;
            return true;
        }
        // clc
        case 0xf8: {
            state.flags.carry = false;
            return true;
        }
        // stc
        case 0xf9: {
            state.flags.carry = true;
            return true;
        }
        // cli
        case 0xfa: {
            state.flags.interruptEnable = false;
            return true;
        }
        // sti
        case 0xfb: {
            state.flags.interruptEnable = true;
            return true;
        }
        // cld
        case 0xfc: {
            state.flags.direction = false;
            return true;
        }
        // std
        case 0xfd: {
            state.flags.direction = true;
            return true;
        }
        // shr16 / shl16
        case 0xd3: {
            const Expected<uint8_t> tmp = Fetch<uint8_t>();
            if (!tmp.HasValue()) return false;
            const ModRM modrm = tmp.Get();
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
        // or16
        MathOperation(0x09, |, false, false, 0)
        // and16
        MathOperation(0x21, &, false, false, 0)
        // xor16
        MathOperation(0x31, ^, false, false, 0)
        // add16
        MathOperation(0x01, +, result > UINT16_MAX, (a ^ result) & (b ^ result) & 0x8000, 0)
        // adc16
        MathOperation(0x11, +, result > UINT16_MAX, (a ^ result) & (b ^ result) & 0x8000, state.flags.carry)
        // sub16
        MathOperation(0x29, -, a < b, (a ^ b) & (a ^ result) & 0x8000, 0)
        // sbb16
        MathOperation(0x19, -, a < b, (a ^ b) & (a ^ result) & 0x8000, state.flags.carry)
        // aaa
        case 0x37: {
            if ((state.a.Get8(false) & 0b1111) > 9 || state.flags.auxiliaryCarry) {
                state.a.Set8(state.a.Get8(false) + 6, false);
                state.a.Set8(state.a.Get8(true) + 1, true);
                state.flags.auxiliaryCarry = state.flags.carry = true;
            }
            else state.flags.auxiliaryCarry = state.flags.carry = false;
            state.a.Set8(state.a.Get8(false) & ~0b11110000, false);
            return true;
        }
        // aas
        case 0x3f: {
            if ((state.a.Get8(false) & 0b1111) > 9 || state.flags.auxiliaryCarry) {
                state.a.Set8(state.a.Get8(false) - 6, false);
                state.a.Set8(state.a.Get8(true) - 1, true);
                state.flags.auxiliaryCarry = state.flags.carry = true;
            }
            else state.flags.auxiliaryCarry = state.flags.carry = false;
            state.a.Set8(state.a.Get8(false) & ~0b11110000, false);
            return true;
        }
        // aam
        case 0xd4: {
            const Expected<uint8_t> tmp = Fetch<uint8_t>();
            if (!tmp.HasValue()) return false;
            // TODO: Set Z, S and P
            state.a.Set8(state.a.Get8(false) / tmp.Get(), true);
            state.a.Set8(state.a.Get8(false) % tmp.Get(), false);
            return true;
        }
        // aad
        case 0xd5: {
            const Expected<uint8_t> tmp = Fetch<uint8_t>();
            if (!tmp.HasValue()) return false;
            // TODO: Set Z, S and P
            state.a.Set8(state.a.Get8(true) * tmp.Get() + state.a.Get8(false), false);
            state.a.Set8(0, true);
            return true;
        }
        // extended
        case 0x0f: {
            // jc16
            JumpIf(0x82, state.flags.carry, 16)
            // jnc16
            JumpIf(0x83, !state.flags.carry, 16)
            // jz16 / je16
            JumpIf(0x84, state.flags.zero, 16)
            // jnz16 / jne16
            JumpIf(0x85, !state.flags.zero, 16)
        }
        default: return false;
    }
}