#include <FileSystem/Cipher/TAR/UnixStandardTAR/UnixStandardTAR.hpp>
#include <FileSystem/Cipher/TAR/GNUTAR/GNUTAR.hpp>
#include <FileSystem/Cipher/CipherFileSystem.hpp>
#include <FileSystem/Cipher/Ar/Ar.hpp>
#include <Libc/HostFileSystem.hpp>
#include <iostream>

MathLib::FileCipher* GetFileCipher(const MathLib::Sequence<char>& type) {
    if (type == "GNUTAR"_M) return new MathLib::GNUTAR();
    if (type == "TAR"_M) return new MathLib::TAR();
    if (type == "USTAR"_M) return new MathLib::UnixStandardTAR();
    if (type == "Ar"_M) return new MathLib::Ar();
    return nullptr;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 3 || !(argc % 2)) MathLib::Panic("Usage: "_M + argv[0] + "<file system> <output file> (<input file> <input file name>)*");
        MathLib::HostFileSystem fs;
        MathLib::File file = fs.Open(MathLib::String(argv[2]), MathLib::OpenMode::Write);
        MathLib::CipherFileSystem cipher = MathLib::CipherFileSystem(file, GetFileCipher(MathLib::String(argv[1])), MathLib::CipherKey());
        if (!cipher.Create()) MathLib::Panic("Failed to create file system");
        for (int i = 3; i < argc; i += 2) {
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