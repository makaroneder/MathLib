#include "X86.hpp"
#include "ModRM.hpp"

#define MathOperation(op, operation, cf, of)                                        \
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
            b = src->value;                                                         \
            dst->value operation##= src->value;                                     \
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
X86State::X86State(Flags flags, Register ip) : flags(flags), ip(ip), a(0), b(0), c(0), d(0), si(0), di(0), sp(0), bp(0), r8(0), r9(0), r10(0), r11(0), r12(0), r13(0), r14(0), r15(0) {}
String X86State::ToString(const String& padding) const {
    String ret = flags.carry ? "CF" : "";
    ret += flags.parity ? (String(ret.IsEmpty() ? "" : ", ") + "PF") : "";
    ret += flags.auxiliaryCarry ? (String(ret.IsEmpty() ? "" : ", ") + "AF") : "";
    ret += flags.zero ? (String(ret.IsEmpty() ? "" : ", ") + "ZF") : "";
    ret += flags.sign ? (String(ret.IsEmpty() ? "" : ", ") + "SF") : "";
    ret += flags.trap ? (String(ret.IsEmpty() ? "" : ", ") + "TF") : "";
    ret += flags.interruptEnable ? (String(ret.IsEmpty() ? "" : ", ") + "IF") : "";
    ret += flags.direction ? (String(ret.IsEmpty() ? "" : ", ") + "DF") : "";
    ret += flags.overflow ? (String(ret.IsEmpty() ? "" : ", ") + "OF") : "";
    ret += flags.resume ? (String(ret.IsEmpty() ? "" : ", ") + "RF") : "";
    ret += flags.virtual8086 ? (String(ret.IsEmpty() ? "" : ", ") + "VM") : "";
    ret += flags.alignmentCheck ? (String(ret.IsEmpty() ? "" : ", ") + "AC") : "";
    ret += flags.virtualInterrupt ? (String(ret.IsEmpty() ? "" : ", ") + "VIF") : "";
    ret += flags.virtualInterruptPending ? (String(ret.IsEmpty() ? "" : ", ") + "VIP") : "";
    ret += flags.id ? (String(ret.IsEmpty() ? "" : ", ") + "ID") : "";
    ret = padding + "{\n" + padding + "\tFlags: " + (ret.IsEmpty() ? "none" : ret) + '\n';
    ret += padding + "\tIP = " + ip.ToString() + '\n';
    ret += padding + "\tSP = " + sp.ToString() + '\n';
    ret += padding + "\tBP = " + bp.ToString() + '\n';
    ret += padding + "\tA = " + a.ToString() + '\n';
    ret += padding + "\tB = " + b.ToString() + '\n';
    ret += padding + "\tC = " + c.ToString() + '\n';
    ret += padding + "\tD = " + d.ToString() + '\n';
    ret += padding + "\tSI = " + si.ToString() + '\n';
    ret += padding + "\tDI = " + di.ToString() + '\n';
    ret += padding + "\tR8 = " + r8.ToString() + '\n';
    ret += padding + "\tR9 = " + r9.ToString() + '\n';
    ret += padding + "\tR10 = " + r10.ToString() + '\n';
    ret += padding + "\tR11 = " + r11.ToString() + '\n';
    ret += padding + "\tR12 = " + r12.ToString() + '\n';
    ret += padding + "\tR13 = " + r13.ToString() + '\n';
    ret += padding + "\tR14 = " + r14.ToString() + '\n';
    ret += padding + "\tR15 = " + r15.ToString() + '\n';
    return ret + padding + '}';
}
X86::X86(const Array<uint8_t>& mem, const X86State& state) : Emulator(mem), state(state) {}
void X86::UpdateFlags(uint64_t val, uint64_t a, uint64_t b) {
    state.flags.zero = !val;
    state.flags.sign = val & 0x8000;
    uint8_t parity = val & UINT8_MAX;
    parity ^= parity >> 4;
    parity ^= parity >> 2;
    parity ^= parity >> 1;
    state.flags.parity = !(parity & 1);
    state.flags.auxiliaryCarry = (a ^ b ^ val) & 0x10;
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
bool X86::Step(void) {
    const Expected<uint8_t> opcode = Fetch<uint8_t>();
    if (!opcode.HasValue()) return false;
    switch (opcode.Get()) {
        // nop
        case 0x90: return true;
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
            return true;
        }
        // dec
        case 0x48 ... 0x4f: {
            Register* reg = GetRegister(opcode.Get() - 0x48);
            if (!reg) return false;
            reg->value--;
            UpdateFlags(reg->value, reg->value + 1, 1);
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
            Expected<uint16_t> tmp = Pop<uint16_t>();
            if (!tmp.HasValue()) return false;
            reg->Set16(tmp.Get(), false);
            return true;
        }
        // call16
        case 0xe8: {
            const Expected<uint16_t> val = Fetch<uint16_t>();
            if (!val.HasValue() || !Push<uint16_t>(state.ip.Get16(false))) return false;
            state.ip.value += val.Get();
            return true;
        }
        // ret16
        case 0xc3: {
            Expected<uint16_t> tmp = Pop<uint16_t>();
            if (!tmp.HasValue()) return false;
            state.ip.Set16(tmp.Get(), false);
            return true;
        }
        // TODO: More jmp based instructions
        // jmp8
        JumpIf(0xeb, true, 8)
        // jmp16
        JumpIf(0xe9, true, 16)
        // jc8
        JumpIf(0x72, state.flags.carry, 8)
        // jnc8
        JumpIf(0x73, !state.flags.carry, 8)
        // jz8 / je8
        JumpIf(0x74, state.flags.zero, 8)
        // jnz8 / jne8
        JumpIf(0x75, !state.flags.zero, 8)
        // stos8
        case 0xaa: {
            if (!WritePositioned<uint8_t>(state.a.Get8(false), state.di.value)) return false;
            if (state.flags.direction) state.di.value--;
            else state.di.value++;
            return true;
        }
        // lods8
        case 0xac: {
            const Expected<uint8_t> tmp = ReadPositioned<uint8_t>(state.si.value);
            if (!tmp.HasValue()) return false;
            state.a.Set8(tmp.Get(), false);
            if (state.flags.direction) state.si.value--;
            else state.si.value++;
            return true;
        }
        // movs8
        case 0xa4: {
            const Expected<uint8_t> tmp = ReadPositioned<uint8_t>(state.si.value);
            if (!tmp.HasValue() || !WritePositioned<uint8_t>(tmp.Get(), state.di.value)) return false;
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
        // clc
        case 0xf8: {
            state.flags.carry = false;
            return true;
        }
        // stc
        case 0xf9: {
            state.flags.carry = false;
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
        MathOperation(0x09, |, false, false)
        // and16
        MathOperation(0x21, &, false, false)
        // xor16
        MathOperation(0x31, ^, false, false)
        // add16
        MathOperation(0x01, +, result > UINT16_MAX, (a ^ result) & (b ^ result) & 0x8000)
        // sub16
        MathOperation(0x29, -, a < b, (a ^ b) & (a ^ result) & 0x8000)
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