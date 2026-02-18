#ifndef MathLib_Cryptography_CompressorLZMA2_H
#define MathLib_Cryptography_CompressorLZMA2_H
#include "../ReadableCipher.hpp"

namespace MathLib {
    struct LZMA2 : ReadableCipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
        [[nodiscard]] virtual Array<uint8_t> DecryptReadablePartial(Readable& readable, const CipherKey& key, const Interval<size_t>& range) const override;

        private:
        [[nodiscard]] size_t GetDictionarySize(uint8_t key) const;
    };
}

#endif