#ifdef __x86_64__
#ifndef Multiboot2_H
#define Multiboot2_H
#include "Multiboot2Info.hpp"
#include "../ACPI/RSDP.hpp"

[[nodiscard]] RSDP* InitMultiboot2(Multiboot2Info* info);

#endif
#endif