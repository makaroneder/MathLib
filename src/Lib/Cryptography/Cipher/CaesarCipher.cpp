#include "CaesarCipher.hpp"

namespace MathLib {
    Expected<CipherKey> CaesarCipher::GetKey(const CipherKey& baseKey) const {
        return baseKey.type == CipherKey::Type::Normal ? Expected<CipherKey>(CipherKey(MakeArray<CipherKey>(CipherKey(ByteArray::ToByteArray<ssize_t>(MakeArray<ssize_t>(1))), baseKey))) : Expected<CipherKey>();
    }
}