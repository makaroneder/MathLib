#ifndef MathLib_Executable_ELF_Dynamic_H
#define MathLib_Executable_ELF_Dynamic_H
#include "../../Interfaces/Formatter.hpp"

namespace MathLib {
    struct ELFDynamic {
        enum class Type : uint64_t {
            None = 0,
            RequiredLibrary,
            PLTSize,
            ProcessorDefined,
            SymbolHashTable,
            StringTable,
            SymbolTable,
            RelocationWithAddend,
            RelocationWithAddendSize,
            OneRelocationWithAddendSize,
            StringTableSize,
            StringTableEntrySize,
            Init,
            Fini,
            SharedObjectName,
            DeprecatedLibrarySearchPath,
            Symbolic,
            Relocation,
            RelocationSize,
            OneRelocationSize,
            PLTType,
            DebugEntry,
            TextSectionModification,
            PLT,
            BindNow,
            InitArray,
            FiniArray,
            InitArraySize,
            FiniArraySize,
            LibrarySearchPath,
            Flags,
            Encoding = 32,
            PreInitArray = Encoding,
            PreInitArraySize,
            SymbolTableSection,
            RelativeRelocationSize,
            RelativeRelocation,
            OneRelativeRelocationSize,
            OSSpecificStart = 0x6000000d,
            OSSpecificEnd = 0x6ffff000,
            GNUPrelinkedTimestamp = 0x6ffffdf5,
            GNUConflictSectionSize,
            GNULibraryListSize,
            Checksum,
            PLTPaddingSize,
            MoveEntry,
            MoveSize,
            Feature1,
            POSFlag1,
            SymbolInfoSize,
            SymbolInfoEntrySize,
            GNUHash = 0x6ffffef5,
            TLSDescriptorPLT,
            TLSDescriptorGOT,
            GNUConflictSection,
            GNULibraryList,
            ConfigInfo,
            DependencyAuditing,
            ObjectAuditing,
            PLTPadding,
            MoveTable,
            SymbolInfo,
            VersionSymbol = 0x6ffffff0,
            RelocationWithAddendCount = 0x6ffffff9,
            RelocationCount = 0x6ffffffa,
            Flags1,
            VersionDefinitionTable,
            VersionDefinitionCount,
            RequiredVersionTable,
            RequiredVersionCount,
            CPUSpecificStart,
            Auxiliary = 0x7ffffffd,
            Filter = 0x7fffffff,
            CPUSpecificEnd = Filter,
        };
        Type type;
        uint64_t value;

        [[nodiscard]] bool operator==(const ELFDynamic& other) const;
        [[nodiscard]] bool operator!=(const ELFDynamic& other) const;
    } __attribute__((packed));
    MakeFormatter(ELFDynamic, self, padding, {
        // TODO: Convert type to string
        const String padd = CollectionToString(padding);
        return "{\n"_M +
            padd + "\tType: 0x" + MathLib::ToString((uint64_t)self.type, 16) + '\n' +
            padd + "\tValue: 0x" + MathLib::ToString(self.value, 16) + '\n' +
        padd + '}';
    });
}

#endif