#ifndef Freestanding
#ifndef MathLib_Libc_HostSocket_H
#define MathLib_Libc_HostSocket_H
#include "../Interfaces/RWDevice.hpp"

namespace MathLib {
    struct HostSocket : RWDevice {
        HostSocket(bool tcp);
        HostSocket(int handle);
        virtual ~HostSocket(void) override;
        [[nodiscard]] virtual size_t ReadSizedBuffer(void* buffer, size_t size) override;
        [[nodiscard]] virtual size_t WriteSizedBuffer(const void* buffer, size_t size) override;
        [[nodiscard]] virtual bool Skip(size_t size) override;
        [[nodiscard]] bool Bind(size_t port);
        [[nodiscard]] HostSocket* GetConnection(void);
        [[nodiscard]] bool ConnectToIPv4(const MathLib::String& ip, size_t port);
        [[nodiscard]] bool ConnectToHost(const MathLib::String& host, size_t port);
        [[nodiscard]] int GetHandle(void) const;

        private:
        int handle;
    };
}

#endif
#endif