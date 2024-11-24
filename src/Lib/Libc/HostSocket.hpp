#ifndef Freestanding
#ifndef Libc_HostSocket_H
#define Libc_HostSocket_H
#include "../Interfaces/Socket.hpp"

namespace MathLib {
    struct HostSocket : Socket {
        HostSocket(void);
        HostSocket(int handle_);
        virtual ~HostSocket(void) override;
        /// @brief Reads data from the buffer
        /// @param buffer Buffer to read data from
        /// @param size Size of the buffer
        /// @return Size of bytes read
        virtual size_t ReadSizedBuffer(void* buffer, size_t size) override;
        /// @brief Writes data to the buffer
        /// @param buffer Buffer to write data to
        /// @param size Size of the buffer
        /// @return Size of written bytes
        virtual size_t WriteSizedBuffer(const void* buffer, size_t size) override;
        virtual bool Bind(int port) override;
        virtual Socket* GetConnection(void) override;
        int GetHandle(void) const;

        private:
        int handle;
    };
}

#endif
#endif