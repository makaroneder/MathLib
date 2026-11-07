#include "HTTP.hpp"

MathLib::String HTTP::GetScheme(void) const {
    return "http";
}
size_t HTTP::GetDefaultPort(void) const {
    return 80;
}
MathLib::String HTTP::Read(MathLib::RWDevice& socket, const MathLib::String& host, const MathLib::String& path) const {
    if (!socket.Puts("GET "_M + (path.IsEmpty() ? "/" : path) + " HTTP/1.1\r\nHost: "_M + host + "\r\nConnection: close\r\n\r\n")) MathLib::Panic("Failed to send GET request");
    return socket.ReadUntil('\0');
}