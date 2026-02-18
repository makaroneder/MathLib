#ifdef __x86_64__
#ifndef WAET_H
#define WAET_H
#include <ACPI/ACPITable.hpp>

struct WAET : MathLib::ACPITable {
    static constexpr const char* expectedSignature = "WAET";
    bool enhancedRTC : 1;
    bool enhancedACPIPMTimer : 1;
    uint32_t reserved : 30;
} __attribute__((packed));

#endif
#endif