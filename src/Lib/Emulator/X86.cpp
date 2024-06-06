#include "X86.hpp"

X86::X86(Array<uint8_t> mem) : Emulator(mem) {}
bool X86::Fetch(uint8_t& out) {
    if (memory.GetSize() <= ip.value) return false;
    out = memory.At(ip.value++);
    return true;
}
Register* X86::GetRegister(uint8_t code) {
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
String X86::ToString(String padding) const {
    String ret;
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
        // mov r8, v8
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
        // mov r16, v16
        case 0xb8 ... 0xbf: {
            opcode -= 0xb8;
            Register* reg = GetRegister(opcode);
            uint8_t val1;
            uint8_t val2;
            if (!reg || !Fetch(val1) || !Fetch(val2)) return false;
            reg->Set16(val2 | (val1 << 8), false);
            return true;
        }
        default: return false;
    }
}