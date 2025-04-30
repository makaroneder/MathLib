#ifndef GNUTAR_H
#define GNUTAR_H
#include "../TAR.hpp"

struct GNUTAR : TAR {
    [[nodiscard]] virtual FileCipherData Identify(MathLib::Readable& device, const MathLib::Sequence<uint64_t>& key, size_t& unpaddedSize) const override;
    [[nodiscard]] virtual MathLib::Array<uint8_t> Encrypt(const MathLib::Sequence<uint8_t>& data, const MathLib::Sequence<uint64_t>& key) const override;
};

#endif