#ifndef MathLib_FileSystem_QuakePak_FileEntry_H
#define MathLib_FileSystem_QuakePak_FileEntry_H
#include "../../Typedefs.hpp"

namespace MathLib {
    struct QuakePakFileEntry {
        char name[56];
        uint32_t position;
        uint32_t size;

        String GetName(void) const;
    } __attribute__((packed));
}

#endif