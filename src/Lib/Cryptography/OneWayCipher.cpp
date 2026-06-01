#include "OneWayCipher.hpp"

namespace MathLib {
    bool OneWayCipher::EncryptWritable(Writable& writable, const Sequence<uint8_t>& data, const CipherKey& key) const {
        return writable.WriteCollection(Encrypt(data, key));
    }
}