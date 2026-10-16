#ifndef MathLib_DER_TagClass_H
#define MathLib_DER_TagClass_H
#include <stdint.h>

enum class DERTagClass : uint8_t {
    Universal = 0,
    Application,
    ContextSpecific,
    Private,
};

#endif