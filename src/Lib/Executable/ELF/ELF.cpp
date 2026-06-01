#include "ELF.hpp"
#include "../../String.hpp"

namespace MathLib {
    bool ELF::Load(ByteDevice& byteDevice) {
        if (!byteDevice.Read<ELFHeader>(header) || !header.IsValid()) return false;
        bool ignoreFirstSection = false;
        uint32_t programHeaderEntries = header.programHeaderEntries;
        if (programHeaderEntries == UINT16_MAX) {
            ignoreFirstSection = true;
            ELFSectionHeader tmp;
            if (!byteDevice.ReadPositioned<ELFSectionHeader>(tmp, header.sectionHeader)) return false;
            programHeaderEntries = tmp.info;
        }
        programs = programHeaderEntries;
        sections = header.sectionHeaderEntries - ignoreFirstSection;
        for (uint32_t i = 0; i < programHeaderEntries; i++) {
            ELFProgramHeader tmp;
            if (!byteDevice.ReadPositioned<ELFProgramHeader>(tmp, header.programHeader + i * header.programHeaderEntrySize)) return false;
            Array<uint8_t> data = tmp.memorySize;
            if (!byteDevice.ReadPositionedBuffer(data.GetValue(), tmp.fileSize, tmp.offset)) return false;
            for (uint64_t i = tmp.fileSize; i < tmp.memorySize; i++) data.AtUnsafe(i) = 0;
            programs.AtUnsafe(i) = ELFProgram(tmp, data);
        }
        for (uint16_t i = ignoreFirstSection; i < header.sectionHeaderEntries; i++)
            if (!byteDevice.ReadPositioned<ELFSectionHeader>(sections.AtUnsafe(i), header.sectionHeader + i * header.sectionHeaderEntrySize)) return false;
        return true;
    }
    ExecutableData ELF::GetCode(void) const {
        uint64_t start = UINT64_MAX;
        uint64_t end = 0;
        for (const ELFProgram& program : programs) {
            if (program.header.type != ELFProgramHeader::Type::Loadable) continue;
            start = Min<uint64_t>(start, program.header.virtualAddress);
            end = Max<uint64_t>(end, program.header.virtualAddress + program.header.memorySize);
        }
        if (start >= end) return ExecutableData();
        ExecutableData ret = ExecutableData(start, end, header.entry);
        for (const ELFProgram& program : programs) {
            if (program.header.type != ELFProgramHeader::Type::Loadable) continue;
            if (!ret.LoadSegment(ExecutableSegment(program.header.virtualAddress, program.data))) return ExecutableData();
        }
        return ret;
    }
    ELFExecutable ELF::Relocate(uintptr_t base, const Function<ELFExecutable, String>& load) const {
        ExecutableData data = GetCode();
        uint64_t dynamic = 0;
        for (const ELFProgram& program : programs) {
            if (program.header.type != ELFProgramHeader::Type::Dynamic) continue;
            if (dynamic) return ELFExecutable();
            dynamic = program.header.virtualAddress;
        }
        if (!dynamic) return ELFExecutable();
        ELFExecutable exec = ELFExecutable(data.data, dynamic, base);
        const size_t size = exec.GetDependencies();
        Array<ELFExecutable> deps = size;
        for (size_t i = 0; i < size; i++) deps.AtUnsafe(i) = load(exec.GetDependency(i));
        return exec.Relocate(deps) ? exec : ELFExecutable();
    }
    String ELF::ToString(const Sequence<char>& padding) const {
        const String padd = CollectionToString(padding);
        const String padd2 = padd + '\t';
        const String padd3 = padd2 + '\t';
        String ret = "{\n"_M + padd + "\tHeader: " + Formatter<ELFHeader>::ToString(header, padd2) + '\n' + padd2 + "Section headers: {\n";
        for (size_t i = 0; i < sections.GetSize(); i++) ret += padd3 + Formatter<ELFSectionHeader>::ToString(sections.AtUnsafe(i), padd3) + '\n';
        ret += padd2 + "}\n" + padd2 + "Program headers: {\n";
        for (size_t i = 0; i < programs.GetSize(); i++) ret += padd3 + programs.AtUnsafe(i).ToString(padd3) + '\n';
        return ret + padd2 + "}\n" + padd + '}';
    }
}