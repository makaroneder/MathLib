#ifndef InterruptDescriptorTableRegister_H
#define InterruptDescriptorTableRegister_H
#include "InterruptDescriptorTable.hpp"

struct InterruptDescriptorTableRegister {
    uint16_t limit;
	InterruptDescriptorTable* base;

    InterruptDescriptorTableRegister(InterruptDescriptorTable* base);
} __attribute__((packed));

#endif