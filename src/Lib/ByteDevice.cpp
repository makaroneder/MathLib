#include "ByteDevice.hpp"

String ByteDevice::ReadUntil(const char& end) {
    String ret;
    const size_t size = GetSize();
    while (Tell() < size) {
        const char chr = Getc();
        if (chr == end) break;
        ret += chr;
    }
    return ret;
}
bool ByteDevice::Puts(const String& str) {
    return WriteBuffer(str.GetValue(), str.GetSize());
}
char ByteDevice::Getc(void) {
    char chr;
    return Read<char>(chr) ? chr : '\0';
}