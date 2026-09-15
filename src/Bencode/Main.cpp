#include "Bencode.hpp"
#include <iostream>

void Main(int argc, char** argv, MathLib::FileSystem& fs) {
    if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <bencoded data>");
    Bencode bencode;
    if (!bencode.LoadFromPath(fs, MathLib::String(argv[1]))) MathLib::Panic("Failed to load bencoded data");
    std::cout << bencode.ToString() << std::endl;
}