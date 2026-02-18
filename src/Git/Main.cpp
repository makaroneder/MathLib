#include "Git.hpp"
#include <Cryptography/Compressor/ZLib.hpp>
#include <FileSystem/Directory.hpp>
#include <Libc/HostFileSystem.hpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        // TODO: Reverse engineer commit object file format
        // TODO: Make commit object containing root tree hash
        // TODO: Store commit object in reference
        // TODO: Store reference in HEAD
        MathLib::HostFileSystem fs;
        MathLib::Directory gitDirectory = MathLib::Directory(fs, "src/TestPrograms/Git"_M);
        Git git = Git(gitDirectory, "a4f3d240edb511e8f6ef25ac66c5265cfcd94240");
        std::cout << git.ListFiles(""_M, 3) << std::endl;
        MathLib::File file = gitDirectory.Open("objects/99/add09dbbf1fe25cfdc091c8359a0c327c32a2f"_M, MathLib::OpenMode::Read);
        const MathLib::Array<uint8_t> buff = MathLib::ZLib().DecryptReadable(file, MathLib::CipherKey());
        std::cout << MathLib::DumpMemory((uintptr_t)buff.GetValue(), buff.GetSize(), 8) << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}