#ifndef MathLib_CRC32_H
#define MathLib_CRC32_H
#include "Interfaces/Collection.hpp"
#include <stdint.h>

namespace MathLib {
    struct CRC32 {
        CRC32(uint32_t magic);
        void Fill(uint32_t magic);
        [[nodiscard]] uint32_t Calculate(const Collection<uint8_t>& data) const;

        private:
        uint32_t table[256];
    };
}

#endif