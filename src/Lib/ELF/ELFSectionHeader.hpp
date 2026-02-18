#ifndef MathLib_ELF_SectionHeader_H
#define MathLib_ELF_SectionHeader_H
#include "../Interfaces/Formatter.hpp"

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

        [[nodiscard]] bool operator==(const ELFSectionHeader& other) const;
        [[nodiscard]] bool operator!=(const ELFSectionHeader& other) const;
    } __attribute__((packed));
    MakeFormatter(ELFSectionHeader, self, padding, {
        String typeStr;
        switch (self.type) {
            case ELFSectionHeader::Type::Unused: {
                typeStr = "Unused";
                break;
            }
            case ELFSectionHeader::Type::ProgramData: {
                typeStr = "Program data";
                break;
            }
            case ELFSectionHeader::Type::SymbolTable: {
                typeStr = "Symbol table";
                break;
            }
            case ELFSectionHeader::Type::StringTable: {
                typeStr = "String table";
                break;
            }
            case ELFSectionHeader::Type::AddendsRelocationEntries: {
                typeStr = "Addends relocation entries";
                break;
            }
            case ELFSectionHeader::Type::SymbolHashTable: {
                typeStr = "Symbol hash table";
                break;
            }
            case ELFSectionHeader::Type::DynamicLinkInformation: {
                typeStr = "Dynamic link information";
                break;
            }
            case ELFSectionHeader::Type::Note: {
                typeStr = "Note";
                break;
            }
            case ELFSectionHeader::Type::BSS: {
                typeStr = "BSS";
                break;
            }
            case ELFSectionHeader::Type::RelocationEntries: {
                typeStr = "Relocation entries";
                break;
            }
            case ELFSectionHeader::Type::SHLib: {
                typeStr = "SHLib";
                break;
            }
            case ELFSectionHeader::Type::DynamicLinkerSymbolTable: {
                typeStr = "Dynamic linker symbol table";
                break;
            }
            case ELFSectionHeader::Type::ConstructorArrayStart: {
                typeStr = "Constructor array start";
                break;
            }
            case ELFSectionHeader::Type::ConstructorArrayEnd: {
                typeStr = "Constructor array end";
                break;
            }
            case ELFSectionHeader::Type::PreConstructorArray: {
                typeStr = "Preconstructor array";
                break;
            }
            case ELFSectionHeader::Type::SectionGroup: {
                typeStr = "Section group";
                break;
            }
            case ELFSectionHeader::Type::ExtendedSectionIndices: {
                typeStr = "Extended section indices";
                break;
            }
            case ELFSectionHeader::Type::ObjectAttributes: {
                typeStr = "Object attributes";
                break;
            }
            case ELFSectionHeader::Type::GNUHashTable: {
                typeStr = "GNU hash table";
                break;
            }
            case ELFSectionHeader::Type::PrelinkLibraryList: {
                typeStr = "Prelink library list";
                break;
            }
            case ELFSectionHeader::Type::DSOChecksum: {
                typeStr = "DSO checksum";
                break;
            }
            case ELFSectionHeader::Type::SunMove: {
                typeStr = "Sun move";
                break;
            }
            case ELFSectionHeader::Type::SunCOMDAT: {
                typeStr = "Sun COMDAT";
                break;
            }
            case ELFSectionHeader::Type::SunSymbolInfo: {
                typeStr = "Sun symbol info";
                break;
            }
            case ELFSectionHeader::Type::SunVersionDefinition: {
                typeStr = "Sun version definition";
                break;
            }
            case ELFSectionHeader::Type::SunVersionRequirement: {
                typeStr = "Sun version requirement";
                break;
            }
            case ELFSectionHeader::Type::SunVersionSymbolTable: {
                typeStr = "Sun version symbol table";
                break;
            }
            case ELFSectionHeader::Type::CPUSpecificStart ... ELFSectionHeader::Type::CPUSpecificEnd: {
                typeStr = "CPU specific";
                break;
            }
            case ELFSectionHeader::Type::ApplicationSpecificStart ... ELFSectionHeader::Type::ApplicationSpecificEnd: {
                typeStr = "Application specific";
                break;
            }
            default: {
                if (IsBetween(self.type, ELFSectionHeader::Type::SunSpecificStart, ELFSectionHeader::Type::SunSpecificEnd)) typeStr = "Sun specific";
                else if (IsBetween(self.type, ELFSectionHeader::Type::OSSpecificStart, ELFSectionHeader::Type::OSSpecificEnd)) typeStr = "OS specific";
                else typeStr = "Unknown";
            }
        }
        return "{\n"_M +
            padding + "\tName: 0x" + MathLib::ToString(self.name, 16) + '\n' +
            padding + "\tType: " + typeStr + " (0x" + MathLib::ToString((uint32_t)self.type, 16) + ")\n" +
            padding + "\tWritable: " + BoolToString(self.writable) + '\n' +
            padding + "\tIn memory: " + BoolToString(self.inMemory) + '\n' +
            padding + "\tExecutable: " + BoolToString(self.executable) + '\n' +
            padding + "\tMergable: " + BoolToString(self.mergable) + '\n' +
            padding + "\tContains strings: " + BoolToString(self.containsStrings) + '\n' +
            padding + "\tHas SHT index: " + BoolToString(self.hasSHTIndex) + '\n' +
            padding + "\tPreserve link order: " + BoolToString(self.preserveLinkOrder) + '\n' +
            padding + "\tOS specific handling: " + BoolToString(self.osSpecificHandling) + '\n' +
            padding + "\tHas thread local data: " + BoolToString(self.hasThreadLocalData) + '\n' +
            padding + "\tCompressed: " + BoolToString(self.compressed) + '\n' +
            padding + "\tRetain: " + BoolToString(self.retain) + '\n' +
            padding + "\tSpecial ordering: " + BoolToString(self.specialOrdering) + '\n' +
            padding + "\tExclude: " + BoolToString(self.exclude) + '\n' +
            padding + "\tAddress: 0x" + MathLib::ToString(self.address, 16) + '\n' +
            padding + "\tOffset: 0x" + MathLib::ToString(self.offset, 16) + '\n' +
            padding + "\tSize: " + Formatter<size_t>::ToString(self.size) + '\n' +
            padding + "\tLink: 0x" + MathLib::ToString(self.link, 16) + '\n' +
            padding + "\tInfo: 0x" + MathLib::ToString(self.info, 16) + '\n' +
            padding + "\tAlignment: " + Formatter<size_t>::ToString(self.alignment) + '\n' +
            padding + "\tEntry size: " + Formatter<size_t>::ToString(self.entrySize) + '\n' +
        padding + '}';
    });
}

#endif