#ifndef Variable_H
#define Variable_H
#include <Typedefs.hpp>

struct Variable {
    MathLib::String name;
    size_t type;
    size_t offset;

    Variable(void);
    Variable(const MathLib::Sequence<char>& name);
    Variable(const MathLib::Sequence<char>& name, size_t type, size_t offset);
    [[nodiscard]] bool operator==(const Variable& other) const;
    [[nodiscard]] bool operator!=(const Variable& other) const;
};

#endif