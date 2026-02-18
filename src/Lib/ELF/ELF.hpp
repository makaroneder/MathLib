#ifndef MathLib_ELF_H
#define MathLib_ELF_H
#include "ELFHeader.hpp"
#include "ELFProgramHeader.hpp"
#include "ELFSectionHeader.hpp"
#include "../Interfaces/Printable.hpp"
#include "../Interfaces/ByteDevice.hpp"

namespace MathLib {
    struct ELF : Printable {
        ELF(ByteDevice& byteDevice);
        [[nodiscard]] bool Reset(void);
        [[nodiscard]] uint16_t GetStringSection(void) const;
        [[nodiscard]] uint32_t GetProgramHeaderEntries(void) const;
        [[nodiscard]] uint16_t GetSectionHeaderEntries(void) const;
        [[nodiscard]] Expected<ELFHeader> ReadHeader(void) const;
        [[nodiscard]] Expected<ELFSectionHeader> ReadSectionHeader(uint16_t i) const;
        [[nodiscard]] Expected<ELFProgramHeader> ReadProgramHeader(uint32_t i) const;
        [[nodiscard]] Array<uint8_t> ReadProgramHeaderData(uint32_t i) const;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;

        private:
        ByteDevice& byteDevice;
        uint64_t sectionHeaderOffset;
        uint64_t programHeaderOffset;
        uint32_t programHeaderEntries;
        uint16_t programHeaderEntrySize;
        uint16_t sectionHeaderEntries;
        uint16_t sectionHeaderEntrySize;
        uint16_t stringSection;
    };
}

#endif