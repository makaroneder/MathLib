#include "ELFHeader.hpp"
#include <String.hpp>
#include <Memory.hpp>

bool ELFHeader::IsValid(void) const {
    return *(const uint32_t*)signature == *(const uint32_t*)expectedSignature && version == expectedVersion && bitMode != BitMode::Invalid && endianness != Endianness::Invalid;
}
MathLib::String ELFHeader::ToString(const MathLib::Sequence<char>& padding) const {
    MathLib::String bitModeStr;
    switch (bitMode) {
        case BitMode::Invalid: {
            bitModeStr = "Invalid";
            break;
        }
        case BitMode::Bit32: {
            bitModeStr = "32 bit";
            break;
        }
        case BitMode::Bit64: {
            bitModeStr = "64 bit";
            break;
        }
        default: bitModeStr = "Unknown";
    }
    MathLib::String endiannessStr;
    switch (endianness) {
        case Endianness::Invalid: {
            endiannessStr = "Invalid";
            break;
        }
        case Endianness::Big: {
            endiannessStr = "Big endian";
            break;
        }
        case Endianness::Little: {
            endiannessStr = "Little endian";
            break;
        }
        default: endiannessStr = "Unknown";
    }
    MathLib::String abiStr;
    switch (abi) {
        case ABI::SystemV: {
            abiStr = "SystemV";
            break;
        }
        case ABI::HPUX: {
            abiStr = "HP-UX";
            break;
        }
        case ABI::NetBSD: {
            abiStr = "NetBSD";
            break;
        }
        case ABI::GNU: {
            abiStr = "GNU";
            break;
        }
        case ABI::Solaris: {
            abiStr = "Solaris";
            break;
        }
        case ABI::AIX: {
            abiStr = "AIX";
            break;
        }
        case ABI::Irix: {
            abiStr = "Irix";
            break;
        }
        case ABI::FreeBSD: {
            abiStr = "FreeBSD";
            break;
        }
        case ABI::TRU64: {
            abiStr = "TRU64";
            break;
        }
        case ABI::Modesto: {
            abiStr = "Modesto";
            break;
        }
        case ABI::OpenBSD: {
            abiStr = "OpenBSD";
            break;
        }
        case ABI::ARMEABI: {
            abiStr = "ARM EABI";
            break;
        }
        case ABI::ARM: {
            abiStr = "ARM";
            break;
        }
        case ABI::Standalone: {
            abiStr = "Standalone";
            break;
        }
        default: abiStr = "Unknown";
    }
    MathLib::String typeStr;
    switch (type) {
        case Type::None: {
            typeStr = "None";
            break;
        }
        case Type::Relocatable: {
            typeStr = "Relocatable";
            break;
        }
        case Type::Executable: {
            typeStr = "Executable";
            break;
        }
        case Type::Dynamic: {
            typeStr = "Dynamic";
            break;
        }
        case Type::Core: {
            typeStr = "Core";
            break;
        }
        case Type::OSSpecificStart ... Type::OSSpecificEnd: {
            typeStr = "OS specific";
            break;
        }
        case Type::CPUSpecificStart ... Type::CPUSpecificEnd: {
            typeStr = "CPU specific";
            break;
        }
        default: typeStr = "Unknown";
    }
    MathLib::String archStr;
    switch (arch) {
        case Arch::None: {
            archStr = "None";
            break;
        }
        case Arch::M32: {
            archStr = "M32";
            break;
        }
        case Arch::Sparc: {
            archStr = "Sparc";
            break;
        }
        case Arch::X86: {
            archStr = "X86";
            break;
        }
        case Arch::MIPS: {
            archStr = "MIPS";
            break;
        }
        case Arch::PowerPC: {
            archStr = "PowerPC";
            break;
        }
        case Arch::ARM: {
            archStr = "ARM";
            break;
        }
        case Arch::SuperH: {
            archStr = "SuperH";
            break;
        }
        case Arch::IA64: {
            archStr = "IA64";
            break;
        }
        case Arch::X86_64: {
            archStr = "X86_64";
            break;
        }
        case Arch::AArch64: {
            archStr = "AArch64";
            break;
        }
        case Arch::RISCV: {
            archStr = "RISC-V";
            break;
        }
        default: archStr = "Unknown";
    }
    return MathLib::CollectionToString(padding) + "{\n" +
        padding + "\tSignature: 0x" + MathLib::ToString(signature[0], 16) + ' ' + signature[1] + signature[2] + signature[3] + '\n' +
        padding + "\tBit mode: " + bitModeStr + " (0x"_M + MathLib::ToString((uint8_t)bitMode, 16) + ")\n" +
        padding + "\tEndianness: " + endiannessStr + " (0x"_M + MathLib::ToString((uint8_t)endianness, 16) + ")\n" +
        padding + "\tHeader version: " + MathLib::ToString(headerVersion, 10) + '\n' +
        padding + "\tABI: " + abiStr + " (0x"_M + MathLib::ToString((uint8_t)abi, 16) + ")\n" +
        padding + "\tABI version: " + MathLib::ToString(abiVersion, 10) + '\n' +
        padding + "\tReserved: 0x" + MathLib::ToString(reserved, 16) + '\n' +
        padding + "\tType: " + typeStr + " (0x"_M + MathLib::ToString((uint16_t)type, 16) + ")\n" +
        padding + "\tArch: " + archStr + " (0x"_M + MathLib::ToString((uint16_t)arch, 16) + ")\n" +
        padding + "\tVersion: " + MathLib::ToString(version, 10) + '\n' +
        padding + "\tEntry: 0x" + MathLib::ToString(entry, 16) + '\n' +
        padding + "\tProgram header: 0x" + MathLib::ToString(programHeader, 16) + '\n' +
        padding + "\tSection header: 0x" + MathLib::ToString(sectionHeader, 16) + '\n' +
        padding + "\tFlags: 0x" + MathLib::ToString(flags, 16) + '\n' +
        padding + "\tHeader size: " + MathLib::ToString(headerSize, 10) + '\n' +
        padding + "\tProgram header entry size: " + MathLib::ToString(programHeaderEntrySize, 10) + '\n' +
        padding + "\tProgram header entries: " + MathLib::ToString(programHeaderEntries, 10) + '\n' +
        padding + "\tSection header entry size: " + MathLib::ToString(sectionHeaderEntrySize, 10) + '\n' +
        padding + "\tSection header entries: " + MathLib::ToString(sectionHeaderEntries, 10) + '\n' +
        padding + "\tString table: 0x" + MathLib::ToString(stringTable, 16) + '\n' +
    padding + '}';
}