#include "XZCRC32Key.hpp"

namespace MathLib {
    const CipherKey xzCRC32Key = CipherKey(MakeArray<CipherKey>(
        CipherKey(ByteArray::ToByteArray<size_t>(32)),
        CipherKey(ByteArray::ToByteArray<CRC::Polynomial>(CRC::Polynomial::CRC32)),
        CipherKey(MakeArray<uint8_t>(true)),
        CipherKey(MakeArray<uint8_t>(true)),
        CipherKey(ByteArray::ToByteArray<uint64_t>(UINT32_MAX)),
        CipherKey(MakeArray<uint8_t>(true))
    ));
}