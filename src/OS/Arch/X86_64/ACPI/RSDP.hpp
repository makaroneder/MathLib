#ifdef __x86_64__
#ifndef RSDP_H
#define RSDP_H
#include <stdint.h>

struct RSDP {
    static constexpr const char* expectedSignature = "RSD PTR ";
    char signature[8];
    uint8_t checksum;
    char oemID[6];
    uint8_t revision;
    uint32_t rsdt;

    [[nodiscard]] bool IsValid(void) const;
} __attribute__((packed));
struct RSDP2 : RSDP {
    uint32_t length;
    uint64_t xsdt;
    uint8_t extendedChecksum;
    uint8_t reserved[3];

    [[nodiscard]] bool IsValid(void) const;
} __attribute__((packed));
[[nodiscard]] RSDP* FindRSDP(void);

#endif
#endif