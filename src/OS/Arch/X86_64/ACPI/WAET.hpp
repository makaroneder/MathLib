#ifndef WAET_H
#define WAET_H
#include "ACPITable.hpp"

struct WAET : ACPITable {
    static constexpr const char* expectedSignature = "WAET";
    bool enhancedRTC : 1;
    bool enhancedACPIPMTimer : 1;
    uint32_t reserved : 30;
} __attribute__((packed));

#endif