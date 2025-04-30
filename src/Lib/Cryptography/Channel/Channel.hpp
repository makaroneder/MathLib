#ifndef MathLib_Cryptography_Channel_H
#define MathLib_Cryptography_Channel_H
#include "../Cipher/NestedCipherData.hpp"

namespace MathLib {
    struct Channel : OneWayCipher {
        Channel(OneWayCipher* channel, NestedCipherData* cipher);
        virtual ~Channel(void) override;
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const override;
        [[nodiscard]] String SendString(const Sequence<char>& data, const Sequence<uint64_t>& key) const;

        private:
        OneWayCipher* channel;
        NestedCipherData* cipher;
    };
}

#endif