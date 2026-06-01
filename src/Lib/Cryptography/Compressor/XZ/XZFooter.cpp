#include "XZFooter.hpp"
#include "XZCRC32Key.hpp"

namespace MathLib {
    XZFooter::XZFooter(void) : crc32(0), indexSize(0), flags(), signature(0) {}
    XZFooter::XZFooter(uint32_t indexSize, const XZFlags& flags) : crc32(), indexSize(indexSize / 4 - 1), flags(flags), signature(expectedSignature) {
        const ExternArray<uint8_t> crcData = ExternArray<uint8_t>((uint8_t*)&this->indexSize, sizeof(uint32_t) + sizeof(uint16_t));
        const Array<uint8_t> tmp = CRC().Encrypt(crcData, xzCRC32Key);
        crc32 = tmp.GetSize() == sizeof(uint32_t) ? *(const uint32_t*)tmp.GetValue() : 0;
    }
    bool XZFooter::IsValid(void) const {
        if (signature != expectedSignature) return false;
        const ExternArray<uint8_t> crcData = ExternArray<uint8_t>((uint8_t*)&indexSize, sizeof(uint32_t) + sizeof(uint16_t));
        const Array<uint8_t> tmp = CRC().Encrypt(crcData, xzCRC32Key);
        return tmp.GetSize() == sizeof(uint32_t) && crc32 == *(const uint32_t*)tmp.GetValue();
    }
    uint64_t XZFooter::GetIndexSize(void) const {
        return (indexSize + 1) * 4;
    }
}