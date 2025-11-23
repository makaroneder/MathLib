#ifndef MathLib_FileSystem_QuakePak_File_H
#define MathLib_FileSystem_QuakePak_File_H
#include "../../Interfaces/Comparable.hpp"
#include <stdint.h>

namespace MathLib {
    struct QuakePakFile;
    struct QuakePakFile : Comparable<QuakePakFile> {
        uint32_t position;
        uint32_t size;
        bool free;

        QuakePakFile(void);
        QuakePakFile(uint32_t position, uint32_t size);

        protected:
        [[nodiscard]] virtual bool Equals(const QuakePakFile& other) const override;
    };
}

#endif