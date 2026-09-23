#ifndef MathLib_Interfaces_RWDevice_H
#define MathLib_Interfaces_RWDevice_H
#include "Readable.hpp"
#include "Writable.hpp"

namespace MathLib {
    struct RWDevice : Readable, Writable {};
}

#endif