#ifndef CRC32_H
#define CRC32_H
#include "Typedefs.hpp"

namespace MathLib {
    struct CRC32 {
        CRC32(uint32_t magic);
        void Fill(uint32_t magic);
        uint32_t Calculate(const Array<uint8_t>& data) const;

        private:
        uint32_t table[256];
    };
}

#endif