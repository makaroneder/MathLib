#ifndef Value_H
#define Value_H
#include <Typedefs.hpp>

struct Value {
    MathLib::String name;
    MathLib::String value;

    Value(void);
    Value(const MathLib::String& name, const MathLib::String& value);
};
[[nodiscard]] Value ReadValue(const MathLib::Sequence<char>& str, size_t& i);

#endif