#ifndef MathLib_DER_H
#define MathLib_DER_H
#include "DERTerm.hpp"

namespace MathLib {
    [[nodiscard]] bool ParseDER(DERTerm& certificate, const Array<uint8_t>& certificateData);
    [[nodiscard]] Array<uint8_t> DecodePEM(const String& data, const String& label, size_t offset);
}

#endif