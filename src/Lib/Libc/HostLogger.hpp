#ifndef Freestanding
#ifndef MathLib_Libc_Logger_H
#define MathLib_Libc_Logger_H
#include "../Interfaces/Writable.hpp"
#include <ostream>

namespace MathLib {
    struct HostLogger : Writable {
        HostLogger(std::ostream& stream);
        [[nodiscard]] virtual size_t WriteSizedBuffer(const void* buffer, size_t size) override;

        private:
        std::ostream& stream;
    };
}

#endif
#endif