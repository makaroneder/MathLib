#include "Bencode.hpp"
#include <Libc/HostFileSystem.hpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <bencoded data>");
        MathLib::HostFileSystem fs;
        Bencode bencode;
        if (!bencode.LoadFromPath(fs, MathLib::String(argv[1]))) MathLib::Panic("Failed to load bencoded data");
        std::cout << bencode.ToString() << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}