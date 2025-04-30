#include "CipherFileSystem.hpp"
#include "TAR/GNUTAR/GNUTAR.hpp"
#include <Libc/HostFileSystem.hpp>
#include <iostream>

// TODO: Move to core library

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2 || argc % 2) MathLib::Panic("Usage: "_M + argv[0] + " <output file> (<input file> <input file name>)*");
        MathLib::HostFileSystem fs;
        MathLib::File file = fs.Open(MathLib::String(argv[1]), MathLib::OpenMode::Write);
        CipherFileSystem cipher = CipherFileSystem(file, new GNUTAR(), MathLib::Array<uint64_t>());
        if (!cipher.Create()) MathLib::Panic("Failed to create file system");
        for (int i = 2; i < argc; i += 2) {
            std::cout << argv[i] << " => " << argv[i + 1] << std::endl;
            MathLib::File inputFile = fs.Open(MathLib::String(argv[i]), MathLib::OpenMode::Read);
            const size_t size = inputFile.GetSize();
            char buff[size];
            if (!inputFile.ReadBuffer(buff, size)) MathLib::Panic("Failed to read data from file");
            MathLib::File outputFile = cipher.Open(MathLib::String(argv[i + 1]), MathLib::OpenMode::Write);
            if (!outputFile.WriteBuffer(buff, size)) MathLib::Panic("Failed to write data to file");
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}