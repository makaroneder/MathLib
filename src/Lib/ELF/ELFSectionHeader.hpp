#ifndef MathLib_ELF_SectionHeader_H
#define MathLib_ELF_SectionHeader_H
#include "../Typedefs.hpp"

namespace MathLib {
    struct ELFSectionHeader {
        enum class Type : uint32_t {
            Unused = 0,
            ProgramData,
            SymbolTable,
            StringTable,
            AddendsRelocationEntries,
            SymbolHashTable,
            DynamicLinkInformation,
            Note,
            BSS,
            RelocationEntries,
            SHLib,
            DynamicLinkerSymbolTable,
            ConstructorArrayStart = 14,
            ConstructorArrayEnd,
            PreConstructorArray,
            SectionGroup,
            ExtendedSectionIndices,
            OSSpecificStart = 0x60000000,
            ObjectAttributes = 0x6ffffff5,
            GNUHashTable,
            PrelinkLibraryList,
            DSOChecksum,
            SunSpecificStart = 0x6ffffffa,
            SunMove = SunSpecificStart,
            SunCOMDAT,
            SunSymbolInfo,
            SunVersionDefinition,
            SunVersionRequirement,
            SunVersionSymbolTable,
            SunSpecificEnd,
            OSSpecificEnd = SunSpecificEnd,
            CPUSpecificStart = 0x70000000,
            CPUSpecificEnd = 0x7fffffff,
            ApplicationSpecificStart,
            ApplicationSpecificEnd = 0x8fffffff,
        };
        uint32_t name;
        Type type;
        bool writable : 1;
        bool inMemory : 1;
        bool executable : 1;
        bool reserved1 : 1;
        bool mergable : 1;
        bool containsStrings : 1;
        bool hasSHTIndex : 1;
        bool preserveLinkOrder : 1;
        bool osSpecificHandling : 1;
        bool isInGroup : 1;
        bool hasThreadLocalData : 1;
        bool compressed : 1;
        uint16_t reserved2 : 10;
        bool retain : 1;
        uint8_t reserved3 : 7;
        bool specialOrdering : 1;
        bool exclude : 1;
        uint32_t reserved4;
        uint64_t address;
        uint64_t offset;
        uint64_t size;
        uint32_t link;
        uint32_t info;
        uint64_t alignment;
        uint64_t entrySize;
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] String ToString(const Sequence<char>& padding = ""_M) const;
    } __attribute__((packed));
}

#endif