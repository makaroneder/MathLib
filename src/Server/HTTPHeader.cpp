#include "HTTPHeader.hpp"

HTTPHeader::HTTPHeader(MathLib::String n, MathLib::String val) {
    name = n;
    value = val;
}
MathLib::String HTTPHeader::ToString(const MathLib::String& padding) const {
    return padding + name + ": " + value;
}