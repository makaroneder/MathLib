#ifndef MathLib_Executable_ELF_Relocation_H
#define MathLib_Executable_ELF_Relocation_H
#include "../../Interfaces/Formatter.hpp"

namespace MathLib {
    struct ELFRelocation {
        enum class Type : uint32_t {
            None = 0,
            Direct64,
            ProgramCounter32,
            GOT32,
            PLT32,
            CopySymbol,
            CreateGOT,
            CreatePLT,
            Relative,
            ProgramCounterRelativeGOT32Entry,
            ZeroExtendedDirect32,
            SignExtendedDirect32,
            ZeroExtendedDirect16,
            SignExtendedProgramCounter16,
            SignExtendedDirect8,
            SignExtendedProgramCounter8,
            ModuleID,
            ModuleTLSOffset,
            InitialTLSOffset64,
            ProgramCounter32GD,
            ProgramCounter32LD,
            TLSOffset,
            ProgramCounter32IE,
            InitialTLSOffset32,
            ProgramCounter64,
            GOT64Offset,
            ProgramCounterRelativeGOT32,
            GOT64,
            ProgramCounterRelativeGOT64Entry,
            ProgramCounterRelativeGOT64,
            GOT64WithPLT,
            GOTRelativePLT64,
            Size32,
            Size64,
            TLSGOTOffset,
            TLSCall,
            TLSDescriptor,
            IndirectProgramBase,
            ProgramBase64,
            LoadGOTEntryRelativeToProgramCounter32 = 41,
            LoadGOTEntryRelativeToProgramCounter32WithRex,
        };
        uint64_t address;
        Type type;
        uint32_t symbol;

        [[nodiscard]] bool operator==(const ELFRelocation& other) const;
        [[nodiscard]] bool operator!=(const ELFRelocation& other) const;
    } __attribute__((packed));
    MakeFormatter(ELFRelocation, self, padding, {
        const String padd = CollectionToString(padding);
        return "{\n"_M +
            padd + "\tAddress: 0x" + MathLib::ToString(self.address, 16) + '\n' +
            padd + "\tType: 0x" + MathLib::ToString((uint32_t)self.type, 16) + '\n' +
            padd + "\tSymbol: 0x" + MathLib::ToString(self.symbol, 16) + '\n' +
        padd + '}';
    });
}

#endif