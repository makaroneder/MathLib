#include "MOS6502.hpp"
#include "MOS6502Opcode.hpp"
#include "MOS6502Instruction.hpp"
#include "MOS6502AddressingMode.hpp"

namespace MathLib {
    MOS6502::MOS6502(const Sequence<uint8_t>& memory) : StepEmulator(memory), pc(0), sp(0), a(0), x(0), y(0), flags(0) {}
    bool MOS6502::Reset(void) {
        flags = MOS6502Flags();
        pc = 0;
        sp = 0;
        a = 0;
        x = 0;
        y = 0;
        return true;
    }
    MathLib::Expected<uint16_t> MOS6502::GetInstructionOperandAddress(uint8_t addressingMode) {
        switch ((MOS6502AddressingMode)addressingMode) {
            case MOS6502AddressingMode::IndirectX: {
                const Expected<uint8_t> tmp = Fetch<uint8_t>();
                return tmp.HasValue() ? ReadPositioned<uint16_t>(tmp.Get() + x) : MathLib::Expected<uint16_t>();
            }
            case MOS6502AddressingMode::ZeroPage: return Fetch<uint8_t>().Convert<uint16_t>();
            case MOS6502AddressingMode::Immediate: return pc++;
            case MOS6502AddressingMode::Absolute: return Fetch<uint16_t>();
            case MOS6502AddressingMode::IndirectY: {
                const Expected<uint8_t> tmp1 = Fetch<uint8_t>();
                if (!tmp1.HasValue()) return Expected<uint16_t>();
                const Expected<uint16_t> tmp2 = ReadPositioned<uint16_t>(tmp1.Get());
                return tmp2.HasValue() ? tmp2.Get() + y : Expected<uint16_t>();
            }
            case MOS6502AddressingMode::ZeroPageX: {
                // TODO: Use y in LDX
                const Expected<uint8_t> tmp = Fetch<uint8_t>();
                return tmp.HasValue() ? tmp.Get() + x : MathLib::Expected<uint16_t>();
            }
            case MOS6502AddressingMode::AbsoluteY: {
                const Expected<uint16_t> tmp = Fetch<uint16_t>();
                return tmp.HasValue() ? tmp.Get() + y : MathLib::Expected<uint16_t>();
            }
            case MOS6502AddressingMode::AbsoluteX: {
                const Expected<uint16_t> tmp = Fetch<uint16_t>();
                return tmp.HasValue() ? tmp.Get() + x : MathLib::Expected<uint16_t>();
            }
            default: return MathLib::Expected<uint16_t>();
        }
    }
    bool MOS6502::Step(void) {
        const Expected<MOS6502Instruction> opcode = Fetch<MOS6502Instruction>();
        if (!opcode.HasValue()) return false;
        const uint8_t addressingMode = opcode.Get().addressingMode;
        switch ((MOS6502Opcode)opcode.Get().value) {
            case MOS6502Opcode::OrAIndirectX:
            case MOS6502Opcode::OrAZeroPage:
            case MOS6502Opcode::OrAImmediate:
            case MOS6502Opcode::OrAAbsolute:
            case MOS6502Opcode::OrAIndirectY:
            case MOS6502Opcode::OrAZeroPageX:
            case MOS6502Opcode::OrAAbsoluteY:
            case MOS6502Opcode::OrAAbsoluteX: {
                const MathLib::Expected<uint16_t> addr = GetInstructionOperandAddress(addressingMode);
                if (!addr.HasValue()) return false;
                const Expected<uint8_t> tmp = ReadPositioned<uint8_t>(addr.Get());
                if (!tmp.HasValue()) return false;
                a |= tmp.Get();
                flags.zero = !a;
                flags.negative = a & (1 << 7);
                return true;
            }
            case MOS6502Opcode::ArithmeticShiftLeftImplied: {
                flags.carry = a & (1 << 7);
                a <<= 1;
                flags.zero = !a;
                flags.negative = a & (1 << 7);
                return true;
            }
            case MOS6502Opcode::ArithmeticShiftLeftZeroPage:
            case MOS6502Opcode::ArithmeticShiftLeftAbsolute:
            case MOS6502Opcode::ArithmeticShiftLeftZeroPageX:
            case MOS6502Opcode::ArithmeticShiftLeftAbsoluteX: {
                const MathLib::Expected<uint16_t> addr = GetInstructionOperandAddress(addressingMode);
                if (!addr.HasValue()) return false;
                const Expected<uint8_t> tmp = ReadPositioned<uint8_t>(addr.Get());
                if (!tmp.HasValue()) return false;
                flags.carry = tmp.Get() & (1 << 7);
                const uint8_t val = tmp.Get() << 1;
                if (!WritePositioned<uint8_t>(val, addr.Get())) return false;
                flags.zero = !val;
                flags.negative = val & (1 << 7);
                return true;
            }
            case MOS6502Opcode::ClearCarry: {
                flags.carry = false;
                return true;
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
        ret += '\n'_M + padding + "\tPC: 0x" + MathLib::ToString(pc, 16, 4) + '\n';
        ret += CollectionToString(padding) + "\tSP: 0x" + MathLib::ToString(sp, 16, 2) + '\n';
        ret += CollectionToString(padding) + "\tA: 0x" + MathLib::ToString(a, 16, 2) + '\n';
        ret += CollectionToString(padding) + "\tX: 0x" + MathLib::ToString(x, 16, 2) + '\n';
        ret += CollectionToString(padding) + "\tY: 0x" + MathLib::ToString(y, 16, 2) + '\n';
        return ret + padding + '}';
    }
}
