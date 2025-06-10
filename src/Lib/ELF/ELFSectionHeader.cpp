#include "ELFSectionHeader.hpp"
#include "../String.hpp"

namespace MathLib {
    String ELFSectionHeader::ToString(const Sequence<char>& padding) const {
        String typeStr;
        switch (type) {
            case Type::Unused: {
                typeStr = "Unused";
                break;
            }
            case Type::ProgramData: {
                typeStr = "Program data";
                break;
            }
            case Type::SymbolTable: {
                typeStr = "Symbol table";
                break;
            }
            case Type::StringTable: {
                typeStr = "String table";
                break;
            }
            case Type::AddendsRelocationEntries: {
                typeStr = "Addends relocation entries";
                break;
            }
            case Type::SymbolHashTable: {
                typeStr = "Symbol hash table";
                break;
            }
            case Type::DynamicLinkInformation: {
                typeStr = "Dynamic link information";
                break;
            }
            case Type::Note: {
                typeStr = "Note";
                break;
            }
            case Type::BSS: {
                typeStr = "BSS";
                break;
            }
            case Type::RelocationEntries: {
                typeStr = "Relocation entries";
                break;
            }
            case Type::SHLib: {
                typeStr = "SHLib";
                break;
            }
            case Type::DynamicLinkerSymbolTable: {
                typeStr = "Dynamic linker symbol table";
                break;
            }
            case Type::ConstructorArrayStart: {
                typeStr = "Constructor array start";
                break;
            }
            case Type::ConstructorArrayEnd: {
                typeStr = "Constructor array end";
                break;
            }
            case Type::PreConstructorArray: {
                typeStr = "Preconstructor array";
                break;
            }
            case Type::SectionGroup: {
                typeStr = "Section group";
                break;
            }
            case Type::ExtendedSectionIndices: {
                typeStr = "Extended section indices";
                break;
            }
            case Type::ObjectAttributes: {
                typeStr = "Object attributes";
                break;
            }
            case Type::GNUHashTable: {
                typeStr = "GNU hash table";
                break;
            }
            case Type::PrelinkLibraryList: {
                typeStr = "Prelink library list";
                break;
            }
            case Type::DSOChecksum: {
                typeStr = "DSO checksum";
                break;
            }
            case Type::SunMove: {
                typeStr = "Sun move";
                break;
            }
            case Type::SunCOMDAT: {
                typeStr = "Sun COMDAT";
                break;
            }
            case Type::SunSymbolInfo: {
                typeStr = "Sun symbol info";
                break;
            }
            case Type::SunVersionDefinition: {
                typeStr = "Sun version definition";
                break;
            }
            case Type::SunVersionRequirement: {
                typeStr = "Sun version requirement";
                break;
            }
            case Type::SunVersionSymbolTable: {
                typeStr = "Sun version symbol table";
                break;
            }
            case Type::CPUSpecificStart ... Type::CPUSpecificEnd: {
                typeStr = "CPU specific";
                break;
            }
            case Type::ApplicationSpecificStart ... Type::ApplicationSpecificEnd: {
                typeStr = "Application specific";
                break;
            }
            default: {
                if (IsBetween(type, Type::SunSpecificStart, Type::SunSpecificEnd)) typeStr = "Sun specific";
                else if (IsBetween(type, Type::OSSpecificStart, Type::OSSpecificEnd)) typeStr = "OS specific";
                else typeStr = "Unknown";
            }
        }
        StartAndReturnFromBenchmark("{\n"_M +
            padding + "\tName: 0x" + MathLib::ToString(name, 16) + '\n' +
            padding + "\tType: " + typeStr + " (0x" + MathLib::ToString((uint32_t)type, 16) + ")\n" +
            padding + "\tWritable: " + MathLib::BoolToString(writable) + '\n' +
            padding + "\tIn memory: " + MathLib::BoolToString(inMemory) + '\n' +
            padding + "\tExecutable: " + MathLib::BoolToString(executable) + '\n' +
            padding + "\tMergable: " + MathLib::BoolToString(mergable) + '\n' +
            padding + "\tContains strings: " + MathLib::BoolToString(containsStrings) + '\n' +
            padding + "\tHas SHT index: " + MathLib::BoolToString(hasSHTIndex) + '\n' +
            padding + "\tPreserve link order: " + MathLib::BoolToString(preserveLinkOrder) + '\n' +
            padding + "\tOS specific handling: " + MathLib::BoolToString(osSpecificHandling) + '\n' +
            padding + "\tHas thread local data: " + MathLib::BoolToString(hasThreadLocalData) + '\n' +
            padding + "\tCompressed: " + MathLib::BoolToString(compressed) + '\n' +
            padding + "\tRetain: " + MathLib::BoolToString(retain) + '\n' +
            padding + "\tSpecial ordering: " + MathLib::BoolToString(specialOrdering) + '\n' +
            padding + "\tExclude: " + MathLib::BoolToString(exclude) + '\n' +
            padding + "\tAddress: 0x" + MathLib::ToString(address, 16) + '\n' +
            padding + "\tOffset: 0x" + MathLib::ToString(offset, 16) + '\n' +
            padding + "\tSize: " + MathLib::ToString(size, 10) + '\n' +
            padding + "\tLink: 0x" + MathLib::ToString(link, 16) + '\n' +
            padding + "\tInfo: 0x" + MathLib::ToString(info, 16) + '\n' +
            padding + "\tAlignment: " + MathLib::ToString(alignment, 10) + '\n' +
            padding + "\tEntry size: " + MathLib::ToString(entrySize, 10) + '\n' +
        padding + '}');
    }
}