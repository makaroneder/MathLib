#include "MOS6502.hpp"
#include "MOS6502Opcode.hpp"
#include "MOS6502Instruction.hpp"

namespace MathLib {
    MOS6502::MOS6502(const Sequence<uint8_t>& memory) : StepEmulator(memory), flags(), pc(), sp(), a(), x(), y() {}
    bool MOS6502::Reset(void) {
        flags = MOS6502Flags();
        pc = Register();
        sp = Register();
        a = Register();
        x = Register();
        y = Register();
        return true;
    }
    bool MOS6502::OrA(uint16_t addr) {
        const Expected<uint8_t> tmp = ReadPositioned<uint8_t>(addr);
        if (!tmp.HasValue()) return false;
        a.Set16(a.Get16(false) | tmp.Get(), false);
        flags.zero = !a.Get16(false);
        flags.negative = a.Get16(false) & (1 << 7);
        return true;
    }
    bool MOS6502::Step(void) {
        const Expected<MOS6502Instruction> opcode = Fetch<MOS6502Instruction>();
        if (!opcode.HasValue()) return false;
        switch ((MOS6502Opcode)opcode.Get().value) {
            case MOS6502Opcode::OrAIndirectX: {
                const Expected<uint8_t> tmp = Fetch<uint8_t>();
                if (!tmp.HasValue()) return false;
                const Expected<uint16_t> addr = ReadPositioned<uint16_t>((uint8_t)(tmp.Get() + x.Get16(false)));
                return addr.HasValue() && OrA(addr.Get());
            }
            case MOS6502Opcode::OrAZeroPage: {
                const Expected<uint8_t> tmp = Fetch<uint8_t>();
                return tmp.HasValue() && OrA(tmp.Get());
            }
            case MOS6502Opcode::OrAImmediate: {
                const uint16_t tmp = pc.Get16(false);
                pc.value++;
                return OrA(tmp);
            }
            default: return false;
        }
        return true;
    }
    Register MOS6502::GetPC(void) const {
        return pc;
    }
    String MOS6502::ToString(const Sequence<char>& padding) const {
        String ret = "{\n";
        ret += CollectionToString(padding) + "\tFlags: ";
        ret += flags.carry ? 'C' : '-';
        ret += flags.zero ? 'Z' : '-';
        ret += flags.interruptDisable ? 'I' : '-';
        ret += flags.decimal ? 'D' : '-';
        ret += flags.breakFlag ? 'B' : '-';
        ret += flags.overflow ? 'V' : '-';
        ret += flags.negative ? 'N' : '-';
        ret += '\n'_M + padding + "\tPC: " + pc.ToString() + '\n';
        ret += CollectionToString(padding) + "\tSP: " + sp.ToString() + '\n';
        ret += CollectionToString(padding) + "\tA: " + a.ToString() + '\n';
        ret += CollectionToString(padding) + "\tX: " + x.ToString() + '\n';
        ret += CollectionToString(padding) + "\tY: " + y.ToString() + '\n';
        return ret + padding + '}';
    }
}
