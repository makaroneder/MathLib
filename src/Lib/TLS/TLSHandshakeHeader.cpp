#include "TLSHandshakeHeader.hpp"

namespace MathLib {
    TLSHandshakeHeader::TLSHandshakeHeader(void) : type(), size() {}
    TLSHandshakeHeader::TLSHandshakeHeader(Type type, Int24 size) : type(type), size(size) {}
}