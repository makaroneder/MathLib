#include "E9.hpp"
#include "IO.hpp"

bool E9::IsPresent(void) {
    return ReadPort<uint8_t>(0xe9) == 0xe9;
}
size_t E9::WriteSizedBuffer(const void* buffer, const size_t& size) {
    uint8_t* buff8 = (uint8_t*)buffer;
    for (size_t i = 0; i < size; i++) WritePort<uint8_t>(0xe9, buff8[i]);
    return size;
}