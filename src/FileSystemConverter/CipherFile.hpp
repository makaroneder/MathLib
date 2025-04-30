#ifndef CipherFile_H
#define CipherFile_H
#include <FileSystem/OpenMode.hpp>
#include <Typedefs.hpp>

struct CipherFile {
    MathLib::String path;
    size_t position;
    size_t size;
    MathLib::OpenMode mode;
    bool free;

    CipherFile(size_t position = SIZE_MAX, size_t size = SIZE_MAX, const MathLib::Sequence<char>& path = ""_M, MathLib::OpenMode mode = MathLib::OpenMode::Read);
    [[nodiscard]] bool operator==(const CipherFile& other) const;
    [[nodiscard]] bool operator!=(const CipherFile& other) const;
};

#endif