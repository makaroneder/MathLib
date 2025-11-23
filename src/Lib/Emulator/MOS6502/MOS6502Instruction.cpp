#include "MOS6502Instruction.hpp"

namespace MathLib {
    bool MOS6502Instruction::operator==(const MOS6502Instruction& other) const {
        return value == other.value;
    }
    bool MOS6502Instruction::operator!=(const MOS6502Instruction& other) const {
        return !(*this == other);
    }
}