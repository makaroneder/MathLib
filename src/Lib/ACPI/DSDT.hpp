#ifndef DSDT_H
#define DSDT_H
#include "ACPITable.hpp"

namespace MathLib {
    struct DSDT : ACPITable {
        static constexpr const char* expectedSignature = "DSDT";
        uint8_t aml[];
    } __attribute__((packed));
}

#endif