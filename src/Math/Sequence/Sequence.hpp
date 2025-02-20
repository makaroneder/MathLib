#ifndef Sequence_H
#define Sequence_H
#include <stddef.h>

template <typename T, typename Index>
struct Sequence {
    [[nodiscard]] virtual size_t GetSize(void) const = 0;
    [[nodiscard]] virtual T At(const Index& index) const = 0;
};

#endif