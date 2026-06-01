#ifndef Freestanding
#ifndef MathLib_Libc_HostSocket_H
#define MathLib_Libc_HostSocket_H
#include "../Interfaces/Socket.hpp"

namespace MathLib {
    struct HostSocket : Socket {
        HostSocket(bool tcp);
        virtual ~HostSocket(void) override;
        [[nodiscard]] virtual size_t ReadSizedBuffer(void* buffer, size_t size) override;
        [[nodiscard]] virtual size_t WriteSizedBuffer(const void* buffer, size_t size) override;
        [[nodiscard]] virtual bool Skip(size_t size) override;
        [[nodiscard]] virtual bool Bind(size_t port) override;
        [[nodiscard]] virtual Socket* GetConnection(void) override;
        [[nodiscard]] virtual bool Connect(const IPv4& ip, size_t port) override;
        [[nodiscard]] int GetHandle(void) const;

        private:
        int handle;
    };
}

#endif
#endif