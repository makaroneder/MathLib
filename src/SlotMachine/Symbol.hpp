#ifndef Symbol_H
#define Symbol_H
#include <Geometry/WavefrontObject.hpp>

struct Symbol : WavefrontObject<num_t> {
    String name;
    Array<size_t> multiplier;

    Symbol(FileSystem& fileSystem, String path, String n, Array<size_t> mult);
    bool operator==(const Symbol& other) const;
};

#endif