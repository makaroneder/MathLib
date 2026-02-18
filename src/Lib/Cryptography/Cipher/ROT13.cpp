#include "ROT13.hpp"

namespace MathLib {
    Expected<CipherKey> ROT13::GetKey(const CipherKey& baseKey) const {
        return baseKey.IsEmpty() ? Expected<CipherKey>(CipherKey(ByteArray::ToByteArray<ssize_t>(MakeArray<ssize_t>(13)))) : Expected<CipherKey>();
    }
}