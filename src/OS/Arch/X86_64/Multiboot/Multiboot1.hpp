#ifdef __x86_64__
#ifndef Multiboot1_H
#define Multiboot1_H
#include "../../../RangeMemoryManager.hpp"
#include "Multiboot1Info.hpp"

void InitMultiboot1(Multiboot1Info* info, RangeMemoryManager& rangeMemoryManager);

#endif
#endif