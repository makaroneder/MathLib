#ifdef __x86_64__
#ifndef PageEntry_H
#define PageEntry_H
#include <Expected.hpp>

struct PageEntry {
    bool present : 1;
    bool writable : 1;
    bool user : 1;
    bool writeThroughCaching : 1;
    bool disableCache : 1;
    bool accessed : 1;
    bool dirty : 1;
    bool huge : 1;
    bool global : 1;
    uint8_t osSpecific1 : 3;
    uint64_t address : 40;
    uint16_t osSpecific2 : 11;
    bool noExecute : 1;

    void MakeUnused(void);
    [[nodiscard]] MathLib::Expected<uintptr_t> GetAddress(void) const;
} __attribute__((packed));

#endif
#endif