#ifndef MathLib_Executable_ELF_RelocationWithAddend_H
#define MathLib_Executable_ELF_RelocationWithAddend_H
#include "ELFRelocation.hpp"

namespace MathLib {
    struct ELFRelocationWithAddend : ELFRelocation {
        int64_t addend;

        [[nodiscard]] bool operator==(const ELFRelocationWithAddend& other) const;
        [[nodiscard]] bool operator!=(const ELFRelocationWithAddend& other) const;
    } __attribute__((packed));
    MakeFormatter(ELFRelocationWithAddend, self, padding, {
        const String padd = CollectionToString(padding);
        return "{\n"_M +
            padd + "\tAddress: 0x" + MathLib::ToString(self.address, 16) + '\n' +
            padd + "\tType: 0x" + MathLib::ToString((uint32_t)self.type, 16) + '\n' +
            padd + "\tSymbol: 0x" + MathLib::ToString(self.symbol, 16) + '\n' +
            padd + "\tAddend: " + MathLib::SignedToString(self.addend, 10) + '\n' +
        padd + '}';
    });
}

#endif