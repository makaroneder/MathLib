#ifdef __x86_64__
#include "InterruptDescriptor.hpp"

InterruptDescriptor::InterruptDescriptor(uintptr_t offset, uint16_t codeSegment, uint8_t attributes) : offset1(offset), codeSegment(codeSegment), interruptStackTableOffset(0), attributes(attributes), offset2(offset >> 16), offset3(offset >> 24), reserved(0) {}

#endif