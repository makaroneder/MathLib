#include "MOS6502.hpp"
#include "MOS6502Opcode.hpp"
#include "MOS6502Instruction.hpp"

namespace MathLib {
    MOS6502::MOS6502(const Sequence<uint8_t>& memory) : StepEmulator(memory), flags(), pc(), sp(), a(), x(), y() {
        EmptyBenchmark
    }
    void MOS6502::Reset(void) {
        StartBenchmark
        flags = MOS6502Flags();
        pc = Register();
        sp = Register();
        a = Register();
        x = Register();
        y = Register();
        EndBenchmark
    }
    bool MOS6502::OrA(uint16_t addr) {
        StartBenchmark
        const Expected<uint8_t> tmp = ReadPositioned<uint8_t>(addr);
        if (!tmp.HasValue()) ReturnFromBenchmark(false);
        a.Set16(a.Get16(false) | tmp.Get(), false);
        flags.zero = !a.Get16(false);
        flags.negative = a.Get16(false) & (1 << 7);
        ReturnFromBenchmark(true);
    }
    bool MOS6502::Step(void) {
        StartBenchmark
        const Expected<MOS6502Instruction> opcode = Fetch<MOS6502Instruction>();
        if (!opcode.HasValue()) ReturnFromBenchmark(false);
        switch ((MOS6502Opcode)opcode.Get().value) {
            case MOS6502Opcode::OrAIndirectX: {
                const Expected<uint8_t> tmp = Fetch<uint8_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false);
                const Expected<uint16_t> addr = ReadPositioned<uint16_t>((uint8_t)(tmp.Get() + x.Get16(false)));
                ReturnFromBenchmark(addr.HasValue() && OrA(addr.Get()));
            }
            case MOS6502Opcode::OrAZeroPage: {
                const Expected<uint8_t> tmp = Fetch<uint8_t>();
                ReturnFromBenchmark(tmp.HasValue() && OrA(tmp.Get()));
            }
            case MOS6502Opcode::OrAImmediate: {
                const uint16_t tmp = pc.Get16(false);
                pc.value++;
                ReturnFromBenchmark(OrA(tmp));
            }
            default: ReturnFromBenchmark(false);
        }
        ReturnFromBenchmark(true);
    }
    Register MOS6502::GetPC(void) const {
        StartAndReturnFromBenchmark(pc);
    }
    String MOS6502::ToString(const Sequence<char>& padding) const {
        StartBenchmark
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
        ReturnFromBenchmark(ret + padding + '}');
    }
}
