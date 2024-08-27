#ifndef GDT_H
#define GDT_H
#include "TSSEntry.hpp"

struct GDT {
    GDTEntry null;
    GDTEntry code;
    GDTEntry data;
} __attribute__((packed));

#endif