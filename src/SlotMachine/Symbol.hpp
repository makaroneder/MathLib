#ifndef Symbol_H
#define Symbol_H
#include <Geometry/WavefrontObject.hpp>

struct Symbol : WavefrontObject<num_t> {
    String name;
    Array<size_t> multiplier;

    Symbol(FileSystem& fileSystem, const String& path, const String& name, const Array<size_t>& multiplier);
    bool operator==(const Symbol& other) const;
};

#endif