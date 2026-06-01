#ifndef MathLib_Executable_ELF_H
#define MathLib_Executable_ELF_H
#include "ELFHeader.hpp"
#include "ELFProgram.hpp"
#include "../Executable.hpp"
#include "ELFExecutable.hpp"
#include "ELFSectionHeader.hpp"
#include "ELFRelocationWithAddend.hpp"
#include "../../Interfaces/ByteDevice.hpp"

namespace MathLib {
    struct ELF : Executable, Printable {
        [[nodiscard]] bool Load(ByteDevice& byteDevice);
        [[nodiscard]] virtual ExecutableData GetCode(void) const override;
        [[nodiscard]] ELFExecutable Relocate(uintptr_t base, const Function<ELFExecutable, String>& load) const;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;

        private:
        ELFHeader header;
        Array<ELFProgram> programs;
        Array<ELFSectionHeader> sections;
    };
}

#endif