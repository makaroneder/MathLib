#include "Value.hpp"
#include <Utils.hpp>

Value::Value(void) {}
Value::Value(const MathLib::String& name, const MathLib::String& value) : name(name), value(value) {}
Value ReadValue(const MathLib::Sequence<char>& str, size_t& i) {
    MathLib::SkipWhiteSpace(str, i);
    MathLib::String name;
    while (!MathLib::IsWhiteSpace(str[i])) name += str[i++];
    MathLib::SkipWhiteSpace(str, i);
    if (str[i++] != '=') return Value();
    MathLib::SkipWhiteSpace(str, i);
    MathLib::String value;
    while (!MathLib::IsWhiteSpace(str[i])) value += str[i++];
    return Value(name, value);
}