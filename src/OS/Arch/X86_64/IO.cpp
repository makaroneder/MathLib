#ifdef __x86_64__
#include "IO.hpp"

template<> void WritePort(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}
template<> void WritePort(uint16_t port, uint16_t value) {
    asm volatile("outw %0, %1" : : "a"(value), "Nd"(port));
}
template<> void WritePort(uint16_t port, uint32_t value) {
    asm volatile("outl %0, %1" : : "a"(value), "Nd"(port));
}
template<> uint8_t ReadPort(uint16_t port) {
    uint8_t value;
    asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}
template<> uint16_t ReadPort(uint16_t port) {
    uint16_t value;
    asm volatile("inw %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}
template<> uint32_t ReadPort(uint16_t port) {
    uint32_t value;
    asm volatile("inl %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}
void IOWait(void) {
    WritePort<uint8_t>(0x80, 0);
}

#endif