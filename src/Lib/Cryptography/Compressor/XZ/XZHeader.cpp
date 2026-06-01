#include "XZHeader.hpp"
#include "XZCRC32Key.hpp"

namespace MathLib {
    XZHeader::XZHeader(void) : flags(), crc32(0) {}
    XZHeader::XZHeader(const XZFlags& flags) : flags(flags) {
        for (uint8_t i = 0; i < SizeOfArray(expectedSignature); i++) signature[i] = expectedSignature[i];
        const Array<uint8_t> tmp = CRC().Encrypt(flags.GetFlags(), xzCRC32Key);
        crc32 = tmp.GetSize() == sizeof(uint32_t) ? *(const uint32_t*)tmp.GetValue() : 0;
    }
    bool XZHeader::IsValid(void) const {
        for (uint8_t i = 0; i < SizeOfArray(expectedSignature); i++)
            if (signature[i] != expectedSignature[i]) return false;
        const Array<uint8_t> tmp = CRC().Encrypt(flags.GetFlags(), xzCRC32Key);
        return tmp.GetSize() == sizeof(uint32_t) && *(const uint32_t*)tmp.GetValue() == crc32;
    }
}