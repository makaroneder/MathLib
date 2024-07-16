#include "HTTPHeader.hpp"

HTTPHeader::HTTPHeader(String n, String val) {
    name = n;
    value = val;
}
String HTTPHeader::ToString(const String& padding) const {
    return padding + name + ": " + value;
}