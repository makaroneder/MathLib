#include "ELFHeader.hpp"
#include "ELFSectionHeader.hpp"
#include "ELFProgramHeader.hpp"
#include <Libc/HostFileSystem.hpp>
#include <FileSystem/File.hpp>
#include <CommandLine.hpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        const MathLib::CommandLine cmdLine = MathLib::CommandLine(argc, (const char**)argv);
        MathLib::HostFileSystem fs;
        MathLib::File file = fs.Open(cmdLine.GetEntry("program"_M).Get("No program provided"), MathLib::OpenMode::Read);
        ELFHeader header = file.Read<ELFHeader>().Get("Failed to read ELF header");
        if (!header.IsValid()) MathLib::Panic("ELF header is not valid");
        std::cout << "ELF header: " << header.ToString() << std::endl;
        uint32_t programHeaders = header.programHeaderEntries;
        if (!file.Seek(header.sectionHeader, MathLib::SeekMode::Set)) MathLib::Panic("Failed to set file position");
        for (size_t i = 0; i < header.sectionHeaderEntries; i++) {
            uint8_t buff[header.sectionHeaderEntrySize];
            if (!file.ReadBuffer(buff, header.sectionHeaderEntrySize)) MathLib::Panic("Failed to read ELF section header");
            ELFSectionHeader* sectionHeader = (ELFSectionHeader*)buff;
            if (!i && programHeaders == UINT16_MAX) programHeaders = sectionHeader->info;
            std::cout << "ELF section header " << i << ": " << sectionHeader->ToString() << std::endl;
        }
        if (!file.Seek(header.programHeader, MathLib::SeekMode::Set)) MathLib::Panic("Failed to set file position");
        for (size_t i = 0; i < programHeaders; i++) {
            uint8_t buff[header.programHeaderEntrySize];
            if (!file.ReadBuffer(buff, header.programHeaderEntrySize)) MathLib::Panic("Failed to read ELF program header");
            ELFProgramHeader* programHeader = (ELFProgramHeader*)buff;
            std::cout << "ELF program header " << i << ": " << programHeader->ToString() << std::endl;
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}