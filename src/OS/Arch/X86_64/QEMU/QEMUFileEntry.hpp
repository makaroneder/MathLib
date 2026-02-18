#ifdef __x86_64__
#ifndef QEMUFileEntry_H
#define QEMUFileEntry_H
#include "QEMUSelector.hpp"

struct QEMUFileEntry {
    uint32_t size;
    QEMUSelector selector;
    uint16_t reserved;
    char name[56];
} __attribute__((packed));

#endif
#endif