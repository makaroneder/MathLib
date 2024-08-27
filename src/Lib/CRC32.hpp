#ifndef CRC32_H
#define CRC32_H
#include "Typedefs.hpp"

struct CRC32 {
    CRC32(const uint32_t& magic);
    void Fill(const uint32_t& magic);
    uint32_t Calculate(const Array<uint8_t>& data) const;

    private:
    uint32_t table[256];
};

#endif