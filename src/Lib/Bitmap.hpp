#ifndef Bitmap_H
#define Bitmap_H
#include <Expected.hpp>

struct Bitmap {
    Bitmap(const Array<uint8_t>& array);
    size_t GetSize(void) const;
    Expected<bool> Get(size_t i) const;
    bool Set(size_t i, bool value);

    private:
    Array<uint8_t> array;
};

#endif