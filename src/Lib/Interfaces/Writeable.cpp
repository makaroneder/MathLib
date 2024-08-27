#include "Writeable.hpp"

bool Writeable::WriteBuffer(const void* buffer, const size_t& size) {
    return WriteSizedBuffer(buffer, size) == size;
}
bool Writeable::Puts(const String& str) {
    return WriteBuffer(str.GetValue(), str.GetSize());
}
bool Writeable::Puts(const char* str) {
    size_t size = 0;
    while (str[size++]) {}
    return WriteBuffer(str, size);
}