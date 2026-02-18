#ifdef __x86_64__
#ifndef GDT_H
#define GDT_H
#include "TSSEntry.hpp"
#include "TSS.hpp"

struct GDT {
    GDTEntry null;
    GDTEntry code;
    GDTEntry data;
    TSSEntry tss;

    TSS* GetTSS(void);
} __attribute__((packed));
extern "C" GDT gdt;

#endif
#endif