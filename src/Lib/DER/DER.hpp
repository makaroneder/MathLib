#ifndef MathLib_DER_H
#define MathLib_DER_H
#include "DERTerm.hpp"
#include "../Interfaces/Readable.hpp"

namespace MathLib {
    [[nodiscard]] bool ParseDER(DERTerm& certificate, Readable& certificateData);
    [[nodiscard]] Array<uint8_t> DecodePEM(const String& data, const String& label, size_t offset);
}

#endif