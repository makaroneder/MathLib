#ifndef Symbol_H
#define Symbol_H
#include <Geometry/WavefrontObject.hpp>

struct Symbol : MathLib::WavefrontObject<MathLib::num_t> {
    MathLib::String name;
    MathLib::Array<size_t> multiplier;

    Symbol(MathLib::FileSystem& fileSystem, const MathLib::Sequence<char>& path, const MathLib::Sequence<char>& name, const MathLib::Sequence<size_t>& multiplier);
    [[nodiscard]] bool operator==(const Symbol& other) const;
};

#endif