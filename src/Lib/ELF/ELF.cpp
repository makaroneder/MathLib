#include "ELF.hpp"
#include "../String.hpp"

namespace MathLib {
    ELF::ELF(ByteDevice& byteDevice) : byteDevice(byteDevice) {
        if (!Reset()) Panic("Failed to load ELF file");
    }
    bool ELF::Reset(void) {
        const Expected<ELFHeader> tmp = ReadHeader();
        if (!tmp.HasValue()) return false;
        const ELFHeader header = tmp.Get();
        sectionHeaderOffset = header.sectionHeader;
        sectionHeaderEntries = header.sectionHeaderEntries;
        sectionHeaderEntrySize = header.sectionHeaderEntrySize;
        programHeaderOffset = header.programHeader;
        programHeaderEntries = header.programHeaderEntries;
        programHeaderEntrySize = header.programHeaderEntrySize;
        stringSection = header.stringTable;
        if (programHeaderEntries == UINT16_MAX) {
            const Expected<ELFSectionHeader> tmp = ReadSectionHeader(0);
            if (!tmp.HasValue()) return false;
            programHeaderEntries = tmp.Get().info;
        }
        return true;
    }
    uint16_t ELF::GetStringSection(void) const {
        return stringSection;
    }
    uint32_t ELF::GetProgramHeaderEntries(void) const {
        return programHeaderEntries;
    }
    uint16_t ELF::GetSectionHeaderEntries(void) const {
        return sectionHeaderEntries;
    }
    Expected<ELFHeader> ELF::ReadHeader(void) const {
        ELFHeader header;
        return byteDevice.ReadPositioned<ELFHeader>(header, 0) && header.IsValid() ? header : Expected<ELFHeader>();
    }
    Expected<ELFSectionHeader> ELF::ReadSectionHeader(uint16_t i) const {
        return i < sectionHeaderEntries ? byteDevice.ReadPositioned<ELFSectionHeader>(sectionHeaderOffset + i * sectionHeaderEntrySize) : Expected<ELFSectionHeader>();
    }
    Expected<ELFProgramHeader> ELF::ReadProgramHeader(uint32_t i) const {
        return i < programHeaderEntries ? byteDevice.ReadPositioned<ELFProgramHeader>(programHeaderOffset + i * programHeaderEntrySize) : Expected<ELFProgramHeader>();
    }
    Array<uint8_t> ELF::ReadProgramHeaderData(uint32_t i) const {
        const Expected<ELFProgramHeader> tmp = ReadProgramHeader(i);
        if (!tmp.HasValue()) return Array<uint8_t>();
        Array<uint8_t> ret = Array<uint8_t>(tmp.Get().memorySize);
        if (!byteDevice.ReadPositionedBuffer(ret.GetValue(), tmp.Get().fileSize, programHeaderOffset + i * programHeaderEntrySize)) return Array<uint8_t>();
        for (uint64_t i = tmp.Get().fileSize; i < tmp.Get().memorySize; i++) ret.At(i) = 0;
        return ret;
    }
    String ELF::ToString(const Sequence<char>& padding) const {
        const String padd = CollectionToString(padding);
        const String padd2 = padd + '\t';
        const Expected<ELFHeader> header = ReadHeader();
        if (!header.HasValue()) return "";
        String ret = "{\n"_M + padd + "\tHeader: " + Formatter<ELFHeader>::ToString(header.Get(), padd2) + '\n';
        for (size_t i = 0; i < sectionHeaderEntries; i++) {
            const Expected<ELFSectionHeader> tmp = ReadSectionHeader(i);
            if (!tmp.HasValue()) return "";
            ret += padd2 + "Section header " + Formatter<size_t>::ToString(i) + ": " + Formatter<ELFSectionHeader>::ToString(tmp.Get(), padd2) + '\n';
        }
        for (size_t i = 0; i < programHeaderEntries; i++) {
            const Expected<ELFProgramHeader> tmp = ReadProgramHeader(i);
            if (!tmp.HasValue()) return "";
            ret += padd2 + "Program header " + Formatter<size_t>::ToString(i) + ": " + Formatter<ELFProgramHeader>::ToString(tmp.Get(), padd2) + '\n';
        }
        return ret + padd + '}';
    }
}