#ifdef __x86_64__
#ifndef InterruptDescriptorTable_H
#define InterruptDescriptorTable_H
#include "InterruptDescriptor.hpp"

struct InterruptDescriptorTable {
    InterruptDescriptor descriptors[256];
} __attribute__((packed, aligned(0x10)));

#endif
#endif