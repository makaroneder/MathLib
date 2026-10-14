#ifndef AudioSource_H
#define AudioSource_H
#include <Allocator/Allocatable.hpp>

struct AudioSource : MathLib::Allocatable {
    [[nodiscard]] virtual size_t Read(void* output, size_t frameCount) = 0;
};

#endif