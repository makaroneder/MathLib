#ifdef __x86_64__
#ifndef PageEntry_H
#define PageEntry_H
#include <stdint.h>

struct PageEntry {
    bool present : 1;
    bool writable : 1;
    bool user : 1;
    bool writeThrough : 1;
    bool disableCache : 1;
    bool accessed : 1;
    bool dirty : 1;
    bool largePage : 1;
    bool global : 1;
    uint8_t osSpecific1 : 3;
    uint64_t address : 40;
    uint8_t osSpecific2 : 7;
    uint8_t protectionKey : 4;
    bool noExecute : 1;

    PageEntry(void);
    PageEntry(uintptr_t address, bool writable, bool executable, bool largePage);
} __attribute__((packed));

#endif
#endif