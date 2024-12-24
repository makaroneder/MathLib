#ifndef MathLib_Interfaces_Socket_H
#define MathLib_Interfaces_Socket_H
#include "Readable.hpp"
#include "Writeable.hpp"

namespace MathLib {
    struct Socket : Readable, Writeable {
        virtual bool Bind(int port) = 0;
        virtual Socket* GetConnection(void) = 0;
    };
}

#endif