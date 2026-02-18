#ifndef MathLib_Cryptography_Compressor_BytePairEncoding_H
#define MathLib_Cryptography_Compressor_BytePairEncoding_H
#include "BytePairEncodingGrammar.hpp"
#include "../../ReadableCipher.hpp"

namespace MathLib {
    struct BytePairEncoding : ReadableCipher {
        [[nodiscard]] static BytePairEncodingGrammar GenerateGrammar(const Sequence<uint8_t>& data);
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
        [[nodiscard]] virtual Array<uint8_t> DecryptReadablePartial(Readable& readable, const CipherKey& key, const Interval<size_t>& range) const override;
        
        private:
        [[nodiscard]] Array<uint8_t> DecryptSingle(size_t name, const Sequence<BytePairEncodingSubstitute>& substitutes) const;
    };
}

#endif