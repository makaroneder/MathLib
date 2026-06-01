#include "ELFProgram.hpp"

namespace MathLib {
    ELFProgram::ELFProgram(void) : header(), data() {}
    ELFProgram::ELFProgram(const ELFProgramHeader& header, const Array<uint8_t>& data) : header(header), data(data) {}
    String ELFProgram::ToString(const Sequence<char>& padding) const {
        return Formatter<ELFProgramHeader>::ToString(header, padding);
    }
    bool ELFProgram::Equals(const ELFProgram& other) const {
        return header == other.header && data == other.data;
    }
}