#include "TLSHandshakeHeader.hpp"

TLSHandshakeHeader::TLSHandshakeHeader(void) : type(), size() {}
TLSHandshakeHeader::TLSHandshakeHeader(Type type, Int24 size) : type(type), size(size) {}