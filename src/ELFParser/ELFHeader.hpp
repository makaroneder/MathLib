#ifndef ELFHeader_H
#define ELFHeader_H
#include <Typedefs.hpp>

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
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const;
} __attribute__((packed));

#endif