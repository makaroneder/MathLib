#include <Libc/HostFileSystem.hpp>
#include <CommandLine.hpp>
#include <ELF/ELF.hpp>
#include <iostream>

struct ExecutableSegment {
    MathLib::Array<uint8_t> data;
    uintptr_t address;

    ExecutableSegment(void) : data(), address(0) {}
    ExecutableSegment(uintptr_t start, uintptr_t end) : data(end - start), address(start) {}
    ExecutableSegment(uintptr_t address, const MathLib::Sequence<uint8_t>& data) : data(MathLib::CollectionToArray<uint8_t>(data)), address(address) {}
    ExecutableSegment Merge(const ExecutableSegment& other) const {
        const uintptr_t end1 = address + data.GetSize();
        const uintptr_t end2 = other.address + other.data.GetSize();
        ExecutableSegment ret = ExecutableSegment(MathLib::Min<uintptr_t>(address, other.address), MathLib::Max<uintptr_t>(end1, end2));
        for (uint8_t& x : ret.data) x = 0;
        for (uintptr_t i = address; i < end1; i++) ret.data.At(i - ret.address) = data.At(i - address);
        for (uintptr_t i = other.address; i < end2; i++) ret.data.At(i - ret.address) = other.data.At(i - other.address);
        return ret;
    }
    [[nodiscard]] bool operator==(const ExecutableSegment& other) const {
        return address == other.address && data == other.data;
    }
    [[nodiscard]] bool operator!=(const ExecutableSegment& other) const {
        return !(*this == other);
    }
};
ExecutableSegment ToSegment(const MathLib::ELF& elf) {
    const uint32_t programHeaders = elf.GetProgramHeaderEntries();
    ExecutableSegment ret;
    bool first = true;
    for (uint32_t i = 0; i < programHeaders; i++) {
        const MathLib::ELFProgramHeader header = elf.ReadProgramHeader(i).Get();
        if (header.type == MathLib::ELFProgramHeader::Type::Loadable) {
            const ExecutableSegment segment = ExecutableSegment(header.virtualAddress, elf.ReadProgramHeaderData(i));
            ret = first ? segment : ret.Merge(segment);
            first = false;
        }
    }
    return ret;
}

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        const MathLib::CommandLine cmdLine = MathLib::CommandLine(argc, (const char**)argv);
        MathLib::HostFileSystem fs;
        MathLib::File file = fs.Open(cmdLine.GetEntry("program"_M).Get("No program provided"), MathLib::OpenMode::Read);
        const MathLib::ELF elf = MathLib::ELF(file);
        std::cout << "ELF: " << elf << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}