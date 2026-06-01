#ifdef __x86_64__
#include "InterruptDescriptor.hpp"

InterruptDescriptor::InterruptDescriptor(void) : offset1(0), codeSegment(0), interruptStackTableOffset(0), reserved1(0), type(0), reserved3(0), ring(0), present(false), offset2(0), reserved2(0) {}
InterruptDescriptor::InterruptDescriptor(uintptr_t offset, uint16_t codeSegment, InterruptDescriptor::GateType gateType) : offset1(offset), codeSegment(codeSegment), interruptStackTableOffset(0), reserved1(0), type((uint8_t)gateType), reserved3(0), ring(0), present(true), offset2(offset >> 16), reserved2(0) {}

#endif