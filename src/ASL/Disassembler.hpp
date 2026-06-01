#ifndef Disassembler_H
#define Disassembler_H
#include <Interfaces/Readable.hpp>

[[nodiscard]] MathLib::String Disassemble(MathLib::Readable& readable);

#endif