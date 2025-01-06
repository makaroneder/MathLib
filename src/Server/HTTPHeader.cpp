#include "HTTPHeader.hpp"

HTTPHeader::HTTPHeader(MathLib::String name, MathLib::String value) : name(name), value(value) {}
MathLib::String HTTPHeader::ToString(const MathLib::String& padding) const {
    return padding + name + ": " + value;
}