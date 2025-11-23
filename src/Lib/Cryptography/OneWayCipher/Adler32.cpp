#include "Adler32.hpp"

namespace MathLib {
    uint32_t Adler32::Get(const Sequence<uint8_t>& data) const {
        uint32_t a = 1;
        uint32_t b = 0;
        const size_t size = data.GetSize();
        for (size_t i = 0; i < size; i++) {
            a = (a + data.At(i)) % modulo;
            b = (b + a) % modulo;
        }
        return (b << 16) | a;
    }
    Array<uint8_t> Adler32::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        if (!key.IsEmpty()) return Array<uint8_t>();
        const uint32_t ret = Get(data);
        return Array<uint8_t>((const uint8_t*)&ret, sizeof(uint32_t));
    }
}