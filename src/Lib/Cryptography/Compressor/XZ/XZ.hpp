#ifndef MathLib_Cryptography_Compressor_XZ_H
#define MathLib_Cryptography_Compressor_XZ_H
#include "XZRecord.hpp"
#include "../../Cipher.hpp"

namespace MathLib {
    struct XZ : Cipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
        [[nodiscard]] virtual Array<uint8_t> DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const override;

        private:
        [[nodiscard]] Array<XZRecord> ParseIndexStructure(Readable& readable) const;
        [[nodiscard]] size_t GetVariableLengthInteger(Readable& readable, WritableSequence<uint8_t>& array) const;
    };
}

#endif