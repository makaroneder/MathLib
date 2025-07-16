#include "ELF.hpp"
#include "../String.hpp"

namespace MathLib {
    ELF::ELF(ByteDevice& byteDevice) : byteDevice(byteDevice) {
        StartBenchmark
        if (!Reset()) Panic("Failed to load ELF file");
        EndBenchmark
    }
    bool ELF::Reset(void) {
        StartBenchmark
        const Expected<ELFHeader> tmp = ReadHeader();
        if (!tmp.HasValue()) ReturnFromBenchmark(false);
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
            if (!tmp.HasValue()) ReturnFromBenchmark(false);
            programHeaderEntries = tmp.Get().info;
        }
        ReturnFromBenchmark(true);
    }
    uint16_t ELF::GetStringSection(void) const {
        StartAndReturnFromBenchmark(stringSection);
    }
    uint32_t ELF::GetProgramHeaderEntries(void) const {
        StartAndReturnFromBenchmark(programHeaderEntries);
    }
    uint16_t ELF::GetSectionHeaderEntries(void) const {
        StartAndReturnFromBenchmark(sectionHeaderEntries);
    }
    Expected<ELFHeader> ELF::ReadHeader(void) const {
        StartBenchmark
        ELFHeader header;
        ReturnFromBenchmark(byteDevice.ReadPositioned<ELFHeader>(header, 0) && header.IsValid() ? header : Expected<ELFHeader>());
    }
    Expected<ELFSectionHeader> ELF::ReadSectionHeader(uint16_t i) const {
        StartAndReturnFromBenchmark(i < sectionHeaderEntries ? byteDevice.ReadPositioned<ELFSectionHeader>(sectionHeaderOffset + i * sectionHeaderEntrySize) : Expected<ELFSectionHeader>());
    }
    Expected<ELFProgramHeader> ELF::ReadProgramHeader(uint32_t i) const {
        StartAndReturnFromBenchmark(i < programHeaderEntries ? byteDevice.ReadPositioned<ELFProgramHeader>(programHeaderOffset + i * programHeaderEntrySize) : Expected<ELFProgramHeader>());
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
        StartBenchmark
        const String padd = CollectionToString(padding);
        const String padd2 = padd + '\t';
        const Expected<ELFHeader> header = ReadHeader();
        if (!header.HasValue()) ReturnFromBenchmark("");
        String ret = "{\n"_M + padd + "\tHeader: " + header.Get().ToString(padd2) + '\n';
        for (size_t i = 0; i < sectionHeaderEntries; i++) {
            const Expected<ELFSectionHeader> tmp = ReadSectionHeader(i);
            if (!tmp.HasValue()) ReturnFromBenchmark("");
            ret += padd2 + "Section header " + MathLib::ToString(i, 10) + ": " + tmp.Get().ToString(padd2) + '\n';
        }
        for (size_t i = 0; i < programHeaderEntries; i++) {
            const Expected<ELFProgramHeader> tmp = ReadProgramHeader(i);
            if (!tmp.HasValue()) ReturnFromBenchmark("");
            ret += padd2 + "Program header " + MathLib::ToString(i, 10) + ": " + tmp.Get().ToString(padd2) + '\n';
        }
        ReturnFromBenchmark(ret + padd + '}');
    }
}