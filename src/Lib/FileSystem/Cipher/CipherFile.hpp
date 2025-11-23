#ifndef MathLib_FileSystem_Cipher_File_H
#define MathLib_FileSystem_Cipher_File_H
#include "../../Typedefs.hpp"
#include "../OpenMode.hpp"

namespace MathLib {
    struct CipherFile {
        String path;
        size_t position;
        size_t size;
        OpenMode mode;
        bool free;
        
        CipherFile(size_t position = SIZE_MAX, size_t size = SIZE_MAX, const Sequence<char>& path = ""_M, OpenMode mode = OpenMode::Read);
        [[nodiscard]] bool operator==(const CipherFile& other) const;
        [[nodiscard]] bool operator!=(const CipherFile& other) const;
    };
}

#endif