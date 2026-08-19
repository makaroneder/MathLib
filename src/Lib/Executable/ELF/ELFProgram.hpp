#ifndef MathLib_Executable_ELF_Program_H
#define MathLib_Executable_ELF_Program_H
#include "ELFProgramHeader.hpp"
#include "../../Interfaces/Printable.hpp"

namespace MathLib {
    struct ELFProgram;
    struct ELFProgram : Comparable<ELFProgram>, Printable {
        ELFProgramHeader header;
        Array<uint8_t> data;

        ELFProgram(void);
        ELFProgram(const ELFProgramHeader& header, const Array<uint8_t>& data);
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
        [[nodiscard]] virtual bool Equals(const ELFProgram& other) const override;
    };
}

#endif