#ifndef ELFProgramHeader_H
#define ELFProgramHeader_H
#include <Typedefs.hpp>

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

    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const;
} __attribute__((packed));

#endif