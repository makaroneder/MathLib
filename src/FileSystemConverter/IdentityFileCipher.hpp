#ifndef IdentityFileCipher_H
#define IdentityFileCipher_H
#include "FileCipher.hpp"

struct IdentityFileCipher : FileCipher {
    [[nodiscard]] virtual FileCipherData Identify(MathLib::Readable& device, const MathLib::Sequence<uint64_t>& key, size_t& unpaddedSize) const = 0;
    [[nodiscard]] virtual FileCipherData Identify(MathLib::Readable& device, const MathLib::Sequence<uint64_t>& key) const override;
    [[nodiscard]] virtual MathLib::Array<uint8_t> DecryptReadablePartial(MathLib::Readable& readable, const MathLib::Sequence<uint64_t>& key, const MathLib::Interval<size_t>& range) const override;
};

#endif