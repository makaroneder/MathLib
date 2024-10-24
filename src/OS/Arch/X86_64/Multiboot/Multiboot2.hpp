#ifdef __x86_64__
#ifndef Multiboot2_H
#define Multiboot2_H
#include "../../../RangeMemoryManager.hpp"
#include "Multiboot2Info.hpp"
#include "../ACPI/RSDP.hpp"

RSDP* InitMultiboot2(Multiboot2Info* info, RangeMemoryManager& rangeMemoryManager);

#endif
#endif