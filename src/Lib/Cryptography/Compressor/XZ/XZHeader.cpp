#include "XZHeader.hpp"
#include "../../OneWayCipher/CRC.hpp"

namespace MathLib {
    bool XZHeader::IsValid(void) const {
        for (uint8_t i = 0; i < SizeOfArray(expectedSignature); i++)
            if (signature[i] != expectedSignature[i]) return false;
        const CipherKey crcKey = CipherKey(MakeArray<CipherKey>(
            CipherKey(ByteArray::ToByteArray<size_t>(MakeArray<size_t>(32))),
            CipherKey(ByteArray::ToByteArray<CRC::Polynomial>(MakeArray<CRC::Polynomial>(CRC::Polynomial::CRC32))),
            CipherKey(MakeArray<uint8_t>(true)),
            CipherKey(MakeArray<uint8_t>(true)),
            CipherKey(ByteArray::ToByteArray<uint64_t>(MakeArray<uint64_t>(UINT32_MAX))),
            CipherKey(MakeArray<uint8_t>(true))
        ));
        const Array<uint8_t> tmp = CRC().Encrypt(flags.GetFlags(), crcKey);
        return tmp.GetSize() == sizeof(uint32_t) && *(const uint32_t*)tmp.GetValue() == crc32;
    }
}