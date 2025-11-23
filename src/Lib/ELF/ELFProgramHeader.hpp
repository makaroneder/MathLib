#ifndef MathLib_ELF_ProgramHeader_H
#define MathLib_ELF_ProgramHeader_H
#include "../Interfaces/Formatter.hpp"

namespace MathLib {
    struct ELFProgramHeader {
        enum class Type : uint32_t {
            Unused = 0,
            Loadable,
            Dynamic,
            Interpreter,
            AuxiliaryInfo,
            SHLib,
            Self,
            ThreadLocalStorage,
            OSSpecificStart = 0x60000000,
            GNUEhFrame = 0x6474e550,
            GNUStack,
            GNUReadOnly,
            GNUProperty,
            SunSpecificStart = 0x6ffffffa,
            SunBSS = SunSpecificStart,
            SunStack,
            SunSpecificEnd = 0x6fffffff,
            OSSpecificEnd = SunSpecificEnd,
            CPUSpecificStart = 0x70000000,
            CPUSpecificEnd = 0x7fffffff,
        } type;
        bool executable : 1;
        bool writable : 1;
        bool readable : 1;
        uint32_t flags : 29;
        uint64_t offset;
        uint64_t virtualAddress;
        uint64_t physicalAddress;
        uint64_t fileSize;
        uint64_t memorySize;
        uint64_t alignment;

        [[nodiscard]] bool operator==(const ELFProgramHeader& other) const;
        [[nodiscard]] bool operator!=(const ELFProgramHeader& other) const;
    } __attribute__((packed));
    MakeFormatter(ELFProgramHeader, self, padding, {
        String ret = "{\n"_M + padding + "\tType: ";
        switch (self.type) {
            case ELFProgramHeader::Type::Unused: {
                ret += "Unused";
                break;
            }
            case ELFProgramHeader::Type::Loadable: {
                ret += "Loadable";
                break;
            }
            case ELFProgramHeader::Type::Dynamic: {
                ret += "Dynamic";
                break;
            }
            case ELFProgramHeader::Type::Interpreter: {
                ret += "Interpreter";
                break;
            }
            case ELFProgramHeader::Type::AuxiliaryInfo: {
                ret += "Auxiliary info";
                break;
            }
            case ELFProgramHeader::Type::SHLib: {
                ret += "SHLib";
                break;
            }
            case ELFProgramHeader::Type::Self: {
                ret += "ELF program header";
                break;
            }
            case ELFProgramHeader::Type::ThreadLocalStorage: {
                ret += "Thread local storage";
                break;
            }
            case ELFProgramHeader::Type::GNUEhFrame: {
                ret += "GNU .eh_frame";
                break;
            }
            case ELFProgramHeader::Type::GNUStack: {
                ret += "GNU stack";
                break;
            }
            case ELFProgramHeader::Type::GNUReadOnly: {
                ret += "GNU read only";
                break;
            }
            case ELFProgramHeader::Type::GNUProperty: {
                ret += "GNU property";
                break;
            }
            case ELFProgramHeader::Type::SunBSS: {
                ret += "Sun bss";
                break;
            }
            case ELFProgramHeader::Type::SunStack: {
                ret += "Sun stack";
                break;
            }
            case ELFProgramHeader::Type::CPUSpecificStart ... ELFProgramHeader::Type::CPUSpecificEnd: {
                ret += "CPU specific";
                break;
            }
            default: {
                if (IsBetween(self.type, ELFProgramHeader::Type::SunSpecificStart, ELFProgramHeader::Type::SunSpecificEnd)) ret += "Sun specific";
                else if (IsBetween(self.type, ELFProgramHeader::Type::OSSpecificStart, ELFProgramHeader::Type::OSSpecificEnd)) ret += "OS specific";
                else ret += "Unknown";
            }
        }
        return ret + " (0x"_M + MathLib::ToString((uint32_t)self.type, 16) + ")\n" +
            padding + "\tPermissions: " + (self.readable ? 'r' : '-') + (self.writable ? 'w' : '-') + (self.executable ? 'x' : '-') + '\n' +
            padding + "\tFlags: 0x" + MathLib::ToString(self.flags, 16) + '\n' +
            padding + "\tOffset: 0x" + MathLib::ToString(self.offset, 16) + '\n' +
            padding + "\tVirtual address: 0x" + MathLib::ToString(self.virtualAddress, 16) + '\n' +
            padding + "\tPhysical address: 0x" + MathLib::ToString(self.physicalAddress, 16) + '\n' +
            padding + "\tFile size: " + Formatter<uint64_t>::ToString(self.fileSize) + '\n' +
            padding + "\tMemory size: " + Formatter<uint64_t>::ToString(self.memorySize) + '\n' +
            padding + "\tAlignment: " + Formatter<uint64_t>::ToString(self.alignment) + '\n' +
        padding + '}';
    });
}

#endif