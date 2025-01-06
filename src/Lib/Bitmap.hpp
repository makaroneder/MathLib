#ifndef MathLib_Bitmap_H
#define MathLib_Bitmap_H
#include "Expected.hpp"

namespace MathLib {
    struct Bitmap {
        Bitmap(void);
        Bitmap(size_t size);
        Bitmap(const Array<uint8_t>& array);
        [[nodiscard]] size_t GetSize(void) const;
        [[nodiscard]] Expected<bool> Get(size_t i) const;
        [[nodiscard]] bool Set(size_t i, bool value);

        private:
        Array<uint8_t> array;
    };
}

#endif