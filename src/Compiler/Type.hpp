#ifndef Type_H
#define Type_H
#include <Typedefs.hpp>

struct Type {
    MathLib::String name;
    size_t size;

    Type(void);
    Type(const MathLib::Sequence<char>& name);
    Type(const MathLib::Sequence<char>& name, size_t size);
    [[nodiscard]] bool operator==(const Type& other) const;
    [[nodiscard]] bool operator!=(const Type& other) const;
};

#endif