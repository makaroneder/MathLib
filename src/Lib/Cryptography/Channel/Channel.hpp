#ifndef MathLib_Cryptography_Channel_H
#define MathLib_Cryptography_Channel_H
#include "../Cipher.hpp"

namespace MathLib {
    struct Channel : OneWayCipher {
        Channel(OneWayCipher& channel, Cipher& cipher);
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
        [[nodiscard]] String SendString(const Sequence<char>& data, const CipherKey& key) const;

        private:
        OneWayCipher& channel;
        Cipher& cipher;
    };
}

#endif