#include <Libc/HostFileSystem.hpp>
#include <Executable/ELF/ELF.hpp>
#include <CommandLine.hpp>
#include <iostream>

int main(int argc, char** argv) {
    try {
        const MathLib::CommandLine cmdLine = MathLib::CommandLine(argc, (const char**)argv);
        MathLib::HostFileSystem fs;
        MathLib::File file = fs.Open(cmdLine.GetEntry("program"_M).Get("No program provided"), MathLib::OpenMode::Read);
        MathLib::ELF elf;
        if (!elf.Load(file)) MathLib::Panic("Failed to load ELF file");
        std::cout << "ELF: " << elf << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}