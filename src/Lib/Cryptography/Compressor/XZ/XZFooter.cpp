#include "XZFooter.hpp"
#include "../../OneWayCipher/CRC.hpp"

namespace MathLib {
    bool XZFooter::IsValid(void) const {
        if (signature != expectedSignature) return false;
        const CipherKey crcKey = CipherKey(MakeArray<CipherKey>(
            CipherKey(ByteArray::ToByteArray<size_t>(MakeArray<size_t>(32))),
            CipherKey(ByteArray::ToByteArray<CRC::Polynomial>(MakeArray<CRC::Polynomial>(CRC::Polynomial::CRC32))),
            CipherKey(MakeArray<uint8_t>(true)),
            CipherKey(MakeArray<uint8_t>(true)),
            CipherKey(ByteArray::ToByteArray<uint64_t>(MakeArray<uint64_t>(UINT32_MAX))),
            CipherKey(MakeArray<uint8_t>(true))
        ));
        const ExternArray<uint8_t> crcData = ExternArray<uint8_t>((uint8_t*)&indexSize, sizeof(uint32_t) + sizeof(uint16_t));
        const Array<uint8_t> tmp = CRC().Encrypt(crcData, crcKey);
        return tmp.GetSize() == sizeof(uint32_t) && crc32 == *(const uint32_t*)tmp.GetValue();
    }
    uint64_t XZFooter::GetIndexSize(void) const {
        return (indexSize + 1) * 4;
    }
}