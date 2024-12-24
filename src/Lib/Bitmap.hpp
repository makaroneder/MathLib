#ifndef MathLib_Bitmap_H
#define MathLib_Bitmap_H
#include "Expected.hpp"

namespace MathLib {
    struct Bitmap {
        Bitmap(void);
        Bitmap(size_t size);
        Bitmap(const Array<uint8_t>& array);
        size_t GetSize(void) const;
        Expected<bool> Get(size_t i) const;
        bool Set(size_t i, bool value);

        private:
        Array<uint8_t> array;
    };
}

#endif