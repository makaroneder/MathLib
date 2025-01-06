#ifndef Freestanding
#ifndef MathLib_Libc_Logger_H
#define MathLib_Libc_Logger_H
#include "../Interfaces/Writeable.hpp"
#include <ostream>

namespace MathLib {
    struct HostLogger : Writeable {
        HostLogger(std::ostream& stream);
        /// @brief Writes data to the buffer
        /// @param buffer Buffer to write data to
        /// @param size Size of the buffer
        /// @return Size of written bytes
        [[nodiscard]] virtual size_t WriteSizedBuffer(const void* buffer, size_t size) override;

        private:
        std::ostream& stream;
    };
}

#endif
#endif