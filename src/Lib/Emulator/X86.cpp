#include "X86.hpp"
#include "ModRM.hpp"

#define MathOperation(op, operation, cf, of)                                        \
case op: {                                                                          \
    ModRM modrm;                                                                    \
    if (!Fetch(modrm.value)) return false;                                          \
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
    flags.carry = cf;                                                               \
    flags.overflow = of;                                                            \
    return true;                                                                    \
}
#define JumpIf(op, req)                                                             \
case op: {                                                                          \
    uint8_t val1;                                                                   \
    uint8_t val2;                                                                   \
    if (!Fetch(val1) || !Fetch(val2)) return false;                                 \
    if (req) ip.value += val1 | (val2 << 8);                                        \
    return true;                                                                    \
}
X86::X86(const Array<uint8_t>& mem) : Emulator(mem) {}
void X86::UpdateFlags(uint64_t val, uint64_t a, uint64_t b) {
    flags.zero = val == 0;
    flags.sign = val & 0x8000;
    uint8_t parity = val & UINT8_MAX;
    parity ^= parity >> 4;
    parity ^= parity >> 2;
    parity ^= parity >> 1;
    flags.parity = !(parity & 1);
    flags.auxiliaryCarry = (a ^ b ^ val) & 0x10;
}
Register X86::GetIP(void) const {
    return ip;
}
bool X86::Fetch(uint8_t& out) {
    if (memory.GetSize() <= ip.value) return false;
    out = memory.At(ip.value++);
    return true;
}
bool X86::Push(uint8_t val) {
    if (memory.GetSize() <= sp.value) return false;
    memory.At(--sp.value) = val;
    return true;
}
uint8_t X86::Pop(void) {
    return memory.At(sp.value++);
}
Register* X86::GetRegister(const uint8_t& code) {
    switch (code) {
        case 0: return &a;
        case 1: return &c;
        case 2: return &d;
        case 3: return &b;
        case 4: return &sp;
        case 5: return &bp;
        case 6: return &si;
        case 7: return &di;
        default: return nullptr;
    }
}
String X86::ToString(const String& padding) const {
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
    ret = padding + "flags = " + (ret.IsEmpty() ? "none" : ret) + '\n';
    ret += padding + "ip = " + ip.ToString() + '\n';
    ret += padding + "a = " + a.ToString() + '\n';
    ret += padding + "b = " + b.ToString() + '\n';
    ret += padding + "c = " + c.ToString() + '\n';
    ret += padding + "d = " + d.ToString() + '\n';
    ret += padding + "si = " + si.ToString() + '\n';
    ret += padding + "di = " + di.ToString() + '\n';
    ret += padding + "sp = " + sp.ToString() + '\n';
    ret += padding + "bp = " + bp.ToString() + '\n';
    ret += padding + "r8 = " + r8.ToString() + '\n';
    ret += padding + "r9 = " + r9.ToString() + '\n';
    ret += padding + "r10 = " + r10.ToString() + '\n';
    ret += padding + "r11 = " + r11.ToString() + '\n';
    ret += padding + "r12 = " + r12.ToString() + '\n';
    ret += padding + "r13 = " + r13.ToString() + '\n';
    ret += padding + "r14 = " + r14.ToString() + '\n';
    ret += padding + "r15 = " + r15.ToString() + '\n';
    return ret;
}
bool X86::Step(void) {
    uint8_t opcode;
    if (!Fetch(opcode)) return false;
    switch (opcode) {
        // nop
        case 0x90: return true;
        // mov8
        case 0xb0 ... 0xb7: {
            opcode -= 0xb0;
            bool upper = false;
            if (opcode > 3) {
                opcode -= 4;
                upper = true;
            }
            Register* reg = GetRegister(opcode);
            uint8_t val;
            if (!reg || !Fetch(val)) return false;
            reg->Set8(val, upper);
            return true;
        }
        // mov16
        case 0xb8 ... 0xbf: {
            opcode -= 0xb8;
            Register* reg = GetRegister(opcode);
            uint8_t val1;
            uint8_t val2;
            if (!reg || !Fetch(val1) || !Fetch(val2)) return false;
            reg->Set16(val1 | (val2 << 8), false);
            return true;
        }
        // inc
        case 0x40 ... 0x47: {
            opcode -= 0x40;
            Register* reg = GetRegister(opcode);
            if (!reg) return false;
            reg->value++;
            UpdateFlags(reg->value, reg->value - 1, 1);
            return true;
        }
        // dec
        case 0x48 ... 0x4f: {
            opcode -= 0x48;
            Register* reg = GetRegister(opcode);
            if (!reg) return false;
            reg->value--;
            UpdateFlags(reg->value, reg->value + 1, 1);
            return true;
        }
        // push16
        case 0x50 ... 0x57: {
            opcode -= 0x50;
            Register* reg = GetRegister(opcode);
            return reg && Push(reg->Get8(false)) && Push(reg->Get8(true));
        }
        // pop16
        case 0x58 ... 0x5f: {
            opcode -= 0x58;
            Register* reg = GetRegister(opcode);
            if (!reg) return false;
            reg->Set8(Pop(), true);
            reg->Set8(Pop(), false);
            return true;
        }
        // call16
        case 0xe8: {
            uint8_t val1;
            uint8_t val2;
            if (!Fetch(val1) || !Fetch(val2) || !Push(ip.Get8(false)) || !Push(ip.Get8(true))) return false;
            ip.value += val1 | (val2 << 8);
            return true;
        }
        // ret16
        case 0xc3: {
            ip.Set8(Pop(), true);
            ip.Set8(Pop(), false);
            return true;
        }
        // jmp16
        JumpIf(0xe9, true)
        // cmp16
        case 0x39: {
            ModRM modrm;
            if (!Fetch(modrm.value)) return false;
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
            flags.carry = a < b;
            flags.overflow = (a ^ b) & (a ^ (a - b)) & 0x8000;
            return true;
        }
        // clc
        case 0xf8: {
            flags.carry = false;
            return true;
        }
        // stc
        case 0xf9: {
            flags.carry = false;
            return true;
        }
        // cli
        case 0xfa: {
            flags.interruptEnable = false;
            return true;
        }
        // sti
        case 0xfb: {
            flags.interruptEnable = true;
            return true;
        }
        // cld
        case 0xfc: {
            flags.direction = false;
            return true;
        }
        // std
        case 0xfd: {
            flags.direction = true;
            return true;
        }
        // shr16 / shl16
        case 0xd3: {
            ModRM modrm;
            if (!Fetch(modrm.value) || modrm.src != 0b101) return false;
            uint64_t a;
            uint8_t b = c.Get8(false);
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
            if (modrm.src == 0b101) flags.carry = (a >> (b - 1)) & 0x1;
            else if (modrm.src == 0b100) flags.carry = (a >> (16 - b)) & 0x1;
            else return false;
            if (b == 1) flags.overflow = ((result >> 15) & 0x1) ^ ((result >> 14) & 0x1);
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
            JumpIf(0x82, flags.carry)
            // jnc16
            JumpIf(0x83, !flags.carry)
            // jz16 / je16
            JumpIf(0x84, flags.zero)
            // jnz16 / jne16
            JumpIf(0x85, !flags.zero)
        }
        default: return false;
    }
}