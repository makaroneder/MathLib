#ifndef Libs_Memory_H
#define Libs_Memory_H
#include <stddef.h>

namespace MathLib {
    void MemoryCopy(const void* src, void* dst, size_t size);
}

#endif