#include "E9.hpp"
#include "IO.hpp"

bool E9::ReadBuffer(void* buffer, const size_t& size) {
    uint8_t* buff8 = (uint8_t*)buffer;
    for (size_t i = 0; i < size; i++) buff8[i] = ReadPort<uint8_t>(0xe9);
    return true;
}
bool E9::WriteBuffer(const void* buffer, const size_t& size) {
    uint8_t* buff8 = (uint8_t*)buffer;
    for (size_t i = 0; i < size; i++) WritePort<uint8_t>(0xe9, buff8[i]);
    return true;
}
bool E9::Seek(const ssize_t&, const SeekMode&) {
    return false;
}
size_t E9::Tell(void) const {
    return SIZE_MAX;
}
size_t E9::GetSize(void) const {
    return SIZE_MAX;
}