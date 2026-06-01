#include "Disassembler.hpp"
#include <ACPI/DSDT.hpp>

// TODO: More opcodes
enum class Opcode : uint16_t {
    Zero = 0x00,
    One,
    ExtendedOpcode = 0x5b,
    Ones = 0xff,
    Revision = 0x5b30,
};
bool GetOpcode(MathLib::Readable& readable, uint32_t& size, Opcode& opcode) {
    uint8_t tmp;
    if (!size || !readable.Read<uint8_t>(tmp)) return false;
    size--;
    opcode = (Opcode)tmp;
    if (opcode == Opcode::ExtendedOpcode) {
        if (!size || !readable.Read<uint8_t>(tmp)) return false;
        size--;
        opcode = (Opcode)(((uint16_t)Opcode::ExtendedOpcode << 8) | tmp);
    }
    return true;
}
MathLib::String Disassemble(MathLib::Readable& readable) {
    MathLib::ACPITable table;
    if (!readable.Read<MathLib::ACPITable>(table) || !table.IsValid()) return "";
    for (uint8_t i = 0; i < SizeOfArray(MathLib::DSDT::expectedSignatures); i++)
        if (!table.CheckSignature(MathLib::DSDT::expectedSignatures[i])) return false;
    uint32_t size = table.GetSize();
    MathLib::String ret;
    while (size) {
        Opcode opcode;
        if (!GetOpcode(readable, size, opcode)) return "";
        switch (opcode) {
            case Opcode::Zero: {
                ret += "Zero";
                break;
            }
            case Opcode::One: {
                ret += "One";
                break;
            }
            case Opcode::Ones: {
                ret += "Ones";
                break;
            }
            case Opcode::Revision: {
                ret += "Revision";
                break;
            }
            default: return "";
        }
    }
    return ret;
}