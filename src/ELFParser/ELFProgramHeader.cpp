#include "ELFProgramHeader.hpp"
#include <String.hpp>

MathLib::String ELFProgramHeader::ToString(const MathLib::String& padding) const {
    MathLib::String ret = padding + "{\n";
    ret += padding + "\tType: ";
    switch (type) {
        case Type::Unused: {
            ret += "Unused";
            break;
        }
        case Type::Loadable: {
            ret += "Loadable";
            break;
        }
        case Type::Dynamic: {
            ret += "Dynamic";
            break;
        }
        case Type::Interpreter: {
            ret += "Interpreter";
            break;
        }
        case Type::AuxiliaryInfo: {
            ret += "Auxiliary info";
            break;
        }
        case Type::SHLib: {
            ret += "SHLib";
            break;
        }
        case Type::Self: {
            ret += "ELF program header";
            break;
        }
        case Type::ThreadLocalStorage: {
            ret += "Thread local storage";
            break;
        }
        case Type::GNUEhFrame: {
            ret += "GNU .eh_frame";
            break;
        }
        case Type::GNUStack: {
            ret += "GNU stack";
            break;
        }
        case Type::GNUReadOnly: {
            ret += "GNU read only";
            break;
        }
        case Type::GNUProperty: {
            ret += "GNU property";
            break;
        }
        case Type::SunBSS: {
            ret += "Sun bss";
            break;
        }
        case Type::SunStack: {
            ret += "Sun stack";
            break;
        }
        case Type::CPUSpecificStart ... Type::CPUSpecificEnd: {
            ret += "CPU specific";
            break;
        }
        default: {
            if (IsBetween(type, Type::SunSpecificStart, Type::SunSpecificEnd)) ret += "Sun specific";
            else if (IsBetween(type, Type::OSSpecificStart, Type::OSSpecificEnd)) ret += "OS specific";
            else ret += "Unknown";
        }
    }
    ret += " (0x"_M + MathLib::ToString((uint32_t)type, 16) + ")\n" + padding + "\tExecutable: " + MathLib::BoolToString(executable) + '\n';
    ret += padding + "\tWritable: " + MathLib::BoolToString(Writable) + '\n';
    ret += padding + "\tReadable: " + MathLib::BoolToString(readable) + '\n';
    ret += padding + "\tFlags: 0x" + MathLib::ToString(flags, 16) + '\n';
    ret += padding + "\tOffset: 0x" + MathLib::ToString(offset, 16) + '\n';
    ret += padding + "\tVirtual address: 0x" + MathLib::ToString(virtualAddress, 16) + '\n';
    ret += padding + "\tPhysical address: 0x" + MathLib::ToString(physicalAddress, 16) + '\n';
    ret += padding + "\tFile size: " + MathLib::ToString(fileSize, 10) + '\n';
    ret += padding + "\tMemory size: " + MathLib::ToString(memorySize, 10) + '\n';
    ret += padding + "\tAlignment: " + MathLib::ToString(alignment, 10) + '\n';
    return ret + padding + '}';
}