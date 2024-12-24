#include "ELFHeader.hpp"
#include <String.hpp>
#include <Memory.hpp>

bool ELFHeader::IsValid(void) const {
    return *(const uint32_t*)signature == *(const uint32_t*)expectedSignature && version == expectedVersion && bitMode != BitMode::Invalid && endianness != Endianness::Invalid;
}
MathLib::String ELFHeader::ToString(const MathLib::String& padding) const {
    MathLib::String ret = padding + "{\n";
    ret += padding + "\tSignature: 0x" + MathLib::ToString(signature[0], 16) + ' ' + signature[1] + signature[2] + signature[3] + '\n';
    ret += padding + "\tBit mode: ";
    switch (bitMode) {
        case BitMode::Invalid: {
            ret += "Invalid";
            break;
        }
        case BitMode::Bit32: {
            ret += "32 bit";
            break;
        }
        case BitMode::Bit64: {
            ret += "64 bit";
            break;
        }
        default: ret += "Unknown";
    }
    ret += MathLib::String(" (0x") + MathLib::ToString((uint8_t)bitMode, 16) + ")\n";
    ret += padding + "\tEndianness: ";
    switch (endianness) {
        case Endianness::Invalid: {
            ret += "Invalid";
            break;
        }
        case Endianness::Big: {
            ret += "Big endian";
            break;
        }
        case Endianness::Little: {
            ret += "Little endian";
            break;
        }
        default: ret += "Unknown";
    }
    ret += MathLib::String(" (0x") + MathLib::ToString((uint8_t)endianness, 16) + ")\n";
    ret += padding + "\tHeader version: " + MathLib::ToString(headerVersion, 10) + '\n';
    ret += padding + "\tABI: ";
    switch (abi) {
        case ABI::SystemV: {
            ret += "SystemV";
            break;
        }
        case ABI::HPUX: {
            ret += "HP-UX";
            break;
        }
        case ABI::NetBSD: {
            ret += "NetBSD";
            break;
        }
        case ABI::GNU: {
            ret += "GNU";
            break;
        }
        case ABI::Solaris: {
            ret += "Solaris";
            break;
        }
        case ABI::AIX: {
            ret += "AIX";
            break;
        }
        case ABI::Irix: {
            ret += "Irix";
            break;
        }
        case ABI::FreeBSD: {
            ret += "FreeBSD";
            break;
        }
        case ABI::TRU64: {
            ret += "TRU64";
            break;
        }
        case ABI::Modesto: {
            ret += "Modesto";
            break;
        }
        case ABI::OpenBSD: {
            ret += "OpenBSD";
            break;
        }
        case ABI::ARMEABI: {
            ret += "ARM EABI";
            break;
        }
        case ABI::ARM: {
            ret += "ARM";
            break;
        }
        case ABI::Standalone: {
            ret += "Standalone";
            break;
        }
        default: ret += "Unknown";
    }
    ret += MathLib::String(" (0x") + MathLib::ToString((uint8_t)abi, 16) + ")\n";
    ret += padding + "\tABI version: " + MathLib::ToString(abiVersion, 10) + '\n';
    uint64_t tmp = 0;
    MathLib::MemoryCopy(reserved, &tmp, SizeOfArray(reserved));
    ret += padding + "\tReserved: 0x" + MathLib::ToString(tmp, 16) + '\n';
    ret += padding + "\tType: ";
    switch (type) {
        case Type::None: {
            ret += "None";
            break;
        }
        case Type::Relocatable: {
            ret += "Relocatable";
            break;
        }
        case Type::Executable: {
            ret += "Executable";
            break;
        }
        case Type::Dynamic: {
            ret += "Dynamic";
            break;
        }
        case Type::Core: {
            ret += "Core";
            break;
        }
        case Type::OSSpecificStart ... Type::OSSpecificEnd: {
            ret += "OS specific";
            break;
        }
        case Type::CPUSpecificStart ... Type::CPUSpecificEnd: {
            ret += "CPU specific";
            break;
        }
        default: ret += "Unknown";
    }
    ret += MathLib::String(" (0x") + MathLib::ToString((uint16_t)type, 16) + ")\n";
    ret += padding + "\tArch: ";
    switch (arch) {
        case Arch::None: {
            ret += "None";
            break;
        }
        case Arch::M32: {
            ret += "M32";
            break;
        }
        case Arch::Sparc: {
            ret += "Sparc";
            break;
        }
        case Arch::X86: {
            ret += "X86";
            break;
        }
        case Arch::MIPS: {
            ret += "MIPS";
            break;
        }
        case Arch::PowerPC: {
            ret += "PowerPC";
            break;
        }
        case Arch::ARM: {
            ret += "ARM";
            break;
        }
        case Arch::SuperH: {
            ret += "SuperH";
            break;
        }
        case Arch::IA64: {
            ret += "IA64";
            break;
        }
        case Arch::X86_64: {
            ret += "X86_64";
            break;
        }
        case Arch::AArch64: {
            ret += "AArch64";
            break;
        }
        case Arch::RISCV: {
            ret += "RISC-V";
            break;
        }
        default: ret += "Unknown";
    }
    ret += MathLib::String(" (0x") + MathLib::ToString((uint16_t)arch, 16) + ")\n";
    ret += padding + "\tVersion: " + MathLib::ToString(version, 10) + '\n';
    ret += padding + "\tEntry: 0x" + MathLib::ToString(entry, 16) + '\n';
    ret += padding + "\tProgram header: 0x" + MathLib::ToString(programHeader, 16) + '\n';
    ret += padding + "\tSection header: 0x" + MathLib::ToString(sectionHeader, 16) + '\n';
    ret += padding + "\tFlags: 0x" + MathLib::ToString(flags, 16) + '\n';
    ret += padding + "\tHeader size: " + MathLib::ToString(headerSize, 10) + '\n';
    ret += padding + "\tProgram header entry size: " + MathLib::ToString(programHeaderEntrySize, 10) + '\n';
    ret += padding + "\tProgram header entries: " + MathLib::ToString(programHeaderEntries, 10) + '\n';
    ret += padding + "\tSection header entry size: " + MathLib::ToString(sectionHeaderEntrySize, 10) + '\n';
    ret += padding + "\tSection header entries: " + MathLib::ToString(sectionHeaderEntries, 10) + '\n';
    ret += padding + "\tString table: 0x" + MathLib::ToString(stringTable, 16) + '\n';
    return ret + padding + '}';
}