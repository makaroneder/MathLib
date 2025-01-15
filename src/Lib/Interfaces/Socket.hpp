#ifndef MathLib_Interfaces_Socket_H
#define MathLib_Interfaces_Socket_H
#include "Readable.hpp"
#include "Writable.hpp"

namespace MathLib {
    struct Socket : Readable, Writable {
        [[nodiscard]] virtual bool Bind(int port) = 0;
        [[nodiscard]] virtual Socket* GetConnection(void) = 0;
    };
}

#endif