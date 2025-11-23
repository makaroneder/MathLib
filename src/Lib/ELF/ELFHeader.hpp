#ifndef MathLib_ELF_Header_H
#define MathLib_ELF_Header_H
#include "../Interfaces/Formatter.hpp"

namespace MathLib {
    struct ELFHeader {
        static constexpr const char* expectedSignature = "\177ELF";
        static constexpr uint32_t expectedVersion = 1;
        enum class BitMode : uint8_t {
            Invalid = 0,
            Bit32,
            Bit64,
        };
        enum class Endianness : uint8_t {
            Invalid = 0,
            Little,
            Big,
        };
        enum class ABI : uint8_t {
            None = 0,
            SystemV = None,
            HPUX,
            NetBSD,
            GNU,
            Linux = GNU,
            Solaris = 6,
            AIX,
            Irix,
            FreeBSD,
            TRU64,
            Modesto,
            OpenBSD,
            ARMEABI = 64,
            ARM = 97,
            Standalone = UINT8_MAX,
        };
        enum class Type : uint16_t {
            None = 0,
            Relocatable,
            Executable,
            Dynamic,
            Core,
            OSSpecificStart = 0xfe00,
            OSSpecificEnd = 0xfeff,
            CPUSpecificStart = 0xff00,
            CPUSpecificEnd = UINT16_MAX,
        };
        enum class Arch : uint16_t {
            // TODO:
            None = 0,
            M32,
            Sparc,
            X86,
            MIPS = 0x08,
            PowerPC = 0x14,
            ARM = 0x28,
            SuperH = 0x2a,
            IA64 = 0x32,
            X86_64 = 0x3e,
            AArch64 = 0xb7,
            RISCV = 0xf3,
        };

        char signature[4];
        BitMode bitMode;
        Endianness endianness;
        uint8_t headerVersion;
        ABI abi;
        uint8_t abiVersion;
        uint64_t reserved : 56;
        Type type;
        Arch arch;
        uint32_t version;
        uint64_t entry;
        uint64_t programHeader;
        uint64_t sectionHeader;
        uint32_t flags;
        uint16_t headerSize;
        uint16_t programHeaderEntrySize;
        uint16_t programHeaderEntries;
        uint16_t sectionHeaderEntrySize;
        uint16_t sectionHeaderEntries;
        uint16_t stringTable;

        [[nodiscard]] bool IsValid(void) const;
        [[nodiscard]] bool operator==(const ELFHeader& other) const;
        [[nodiscard]] bool operator!=(const ELFHeader& other) const;
    } __attribute__((packed));
    MakeFormatter(ELFHeader, self, padding, {
        String bitModeStr;
        switch (self.bitMode) {
            case ELFHeader::BitMode::Invalid: {
                bitModeStr = "Invalid";
                break;
            }
            case ELFHeader::BitMode::Bit32: {
                bitModeStr = "32 bit";
                break;
            }
            case ELFHeader::BitMode::Bit64: {
                bitModeStr = "64 bit";
                break;
            }
            default: bitModeStr = "Unknown";
        }
        String endiannessStr;
        switch (self.endianness) {
            case ELFHeader::Endianness::Invalid: {
                endiannessStr = "Invalid";
                break;
            }
            case ELFHeader::Endianness::Big: {
                endiannessStr = "Big endian";
                break;
            }
            case ELFHeader::Endianness::Little: {
                endiannessStr = "Little endian";
                break;
            }
            default: endiannessStr = "Unknown";
        }
        String abiStr;
        switch (self.abi) {
            case ELFHeader::ABI::SystemV: {
                abiStr = "SystemV";
                break;
            }
            case ELFHeader::ABI::HPUX: {
                abiStr = "HP-UX";
                break;
            }
            case ELFHeader::ABI::NetBSD: {
                abiStr = "NetBSD";
                break;
            }
            case ELFHeader::ABI::GNU: {
                abiStr = "GNU";
                break;
            }
            case ELFHeader::ABI::Solaris: {
                abiStr = "Solaris";
                break;
            }
            case ELFHeader::ABI::AIX: {
                abiStr = "AIX";
                break;
            }
            case ELFHeader::ABI::Irix: {
                abiStr = "Irix";
                break;
            }
            case ELFHeader::ABI::FreeBSD: {
                abiStr = "FreeBSD";
                break;
            }
            case ELFHeader::ABI::TRU64: {
                abiStr = "TRU64";
                break;
            }
            case ELFHeader::ABI::Modesto: {
                abiStr = "Modesto";
                break;
            }
            case ELFHeader::ABI::OpenBSD: {
                abiStr = "OpenBSD";
                break;
            }
            case ELFHeader::ABI::ARMEABI: {
                abiStr = "ARM EABI";
                break;
            }
            case ELFHeader::ABI::ARM: {
                abiStr = "ARM";
                break;
            }
            case ELFHeader::ABI::Standalone: {
                abiStr = "Standalone";
                break;
            }
            default: abiStr = "Unknown";
        }
        String typeStr;
        switch (self.type) {
            case ELFHeader::Type::None: {
                typeStr = "None";
                break;
            }
            case ELFHeader::Type::Relocatable: {
                typeStr = "Relocatable";
                break;
            }
            case ELFHeader::Type::Executable: {
                typeStr = "Executable";
                break;
            }
            case ELFHeader::Type::Dynamic: {
                typeStr = "Dynamic";
                break;
            }
            case ELFHeader::Type::Core: {
                typeStr = "Core";
                break;
            }
            case ELFHeader::Type::OSSpecificStart ... ELFHeader::Type::OSSpecificEnd: {
                typeStr = "OS specific";
                break;
            }
            case ELFHeader::Type::CPUSpecificStart ... ELFHeader::Type::CPUSpecificEnd: {
                typeStr = "CPU specific";
                break;
            }
            default: typeStr = "Unknown";
        }
        String archStr;
        switch (self.arch) {
            case ELFHeader::Arch::None: {
                archStr = "None";
                break;
            }
            case ELFHeader::Arch::M32: {
                archStr = "M32";
                break;
            }
            case ELFHeader::Arch::Sparc: {
                archStr = "Sparc";
                break;
            }
            case ELFHeader::Arch::X86: {
                archStr = "X86";
                break;
            }
            case ELFHeader::Arch::MIPS: {
                archStr = "MIPS";
                break;
            }
            case ELFHeader::Arch::PowerPC: {
                archStr = "PowerPC";
                break;
            }
            case ELFHeader::Arch::ARM: {
                archStr = "ARM";
                break;
            }
            case ELFHeader::Arch::SuperH: {
                archStr = "SuperH";
                break;
            }
            case ELFHeader::Arch::IA64: {
                archStr = "IA64";
                break;
            }
            case ELFHeader::Arch::X86_64: {
                archStr = "X86_64";
                break;
            }
            case ELFHeader::Arch::AArch64: {
                archStr = "AArch64";
                break;
            }
            case ELFHeader::Arch::RISCV: {
                archStr = "RISC-V";
                break;
            }
            default: archStr = "Unknown";
        }
        return "{\n"_M +
            padding + "\tSignature: 0x" + MathLib::ToString(self.signature[0], 16) + ' ' + self.signature[1] + self.signature[2] + self.signature[3] + '\n' +
            padding + "\tBit mode: " + bitModeStr + " (0x"_M + MathLib::ToString((uint8_t)self.bitMode, 16) + ")\n" +
            padding + "\tEndianness: " + endiannessStr + " (0x"_M + MathLib::ToString((uint8_t)self.endianness, 16) + ")\n" +
            padding + "\tHeader version: " + Formatter<uint8_t>::ToString(self.headerVersion) + '\n' +
            padding + "\tABI: " + abiStr + " (0x"_M + MathLib::ToString((uint8_t)self.abi, 16) + ")\n" +
            padding + "\tABI version: " + Formatter<uint8_t>::ToString(self.abiVersion) + '\n' +
            padding + "\tReserved: 0x" + MathLib::ToString(self.reserved, 16) + '\n' +
            padding + "\tType: " + typeStr + " (0x"_M + MathLib::ToString((uint16_t)self.type, 16) + ")\n" +
            padding + "\tArch: " + archStr + " (0x"_M + MathLib::ToString((uint16_t)self.arch, 16) + ")\n" +
            padding + "\tVersion: " + Formatter<uint32_t>::ToString(self.version) + '\n' +
            padding + "\tEntry: 0x" + MathLib::ToString(self.entry, 16) + '\n' +
            padding + "\tProgram header: 0x" + MathLib::ToString(self.programHeader, 16) + '\n' +
            padding + "\tSection header: 0x" + MathLib::ToString(self.sectionHeader, 16) + '\n' +
            padding + "\tFlags: 0x" + MathLib::ToString(self.flags, 16) + '\n' +
            padding + "\tHeader size: " + Formatter<uint16_t>::ToString(self.headerSize) + '\n' +
            padding + "\tProgram header entry size: " + Formatter<uint16_t>::ToString(self.programHeaderEntrySize) + '\n' +
            padding + "\tProgram header entries: " + Formatter<uint16_t>::ToString(self.programHeaderEntries) + '\n' +
            padding + "\tSection header entry size: " + Formatter<uint16_t>::ToString(self.sectionHeaderEntrySize) + '\n' +
            padding + "\tSection header entries: " + Formatter<uint16_t>::ToString(self.sectionHeaderEntries) + '\n' +
            padding + "\tString table: 0x" + MathLib::ToString(self.stringTable, 16) + '\n' +
        padding + '}';
    });
}

#endif