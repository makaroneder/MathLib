#include <Interfaces/Sequence/VariadicSequence.hpp>
#include <Emulator/MOS6502/MOS6502Opcode.hpp>
#include <Emulator/MOS6502/MOS6502.hpp>
#include <FileSystem/FileSystem.hpp>
#include <Logger.hpp>

void Main(int, char**, MathLib::FileSystem&) {
    MathLib::MOS6502 mos6502 = MathLib::MOS6502(MathLib::VariadicSequence<uint8_t,
        (uint8_t)MathLib::MOS6502Opcode::OrAZeroPage, 0x00
    >());
    if (!mos6502.Run()) MathLib::Panic("Failed to update emulator");
    LogString("State: ");
    LogString(mos6502.ToString());
    LogChar('\n');
}