#ifndef MathLib_Interfaces_Socket_H
#define MathLib_Interfaces_Socket_H
#include "Readable.hpp"
#include "Writable.hpp"
#include "../IPv4.hpp"

namespace MathLib {
    struct Socket : Readable, Writable {
        [[nodiscard]] virtual bool Bind(size_t port) = 0;
        [[nodiscard]] virtual Socket* GetConnection(void) = 0;
        [[nodiscard]] virtual bool Connect(const IPv4& ip, size_t port) = 0;
    };
}

#endif