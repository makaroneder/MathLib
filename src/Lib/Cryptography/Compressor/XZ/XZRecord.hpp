#ifndef MathLib_Cryptography_Compressor_XZ_Record_H
#define MathLib_Cryptography_Compressor_XZ_Record_H
#include "../../../Interfaces/Comparable.hpp"

namespace MathLib {
    struct XZRecord;
    struct XZRecord : Comparable<XZRecord> {
        size_t unpaddedSize;
        size_t uncompressedSize;

        XZRecord(void);
        XZRecord(size_t unpaddedSize, size_t uncompressedSize);

        protected:
        [[nodiscard]] virtual bool Equals(const XZRecord& other) const override;
    };
}

#endif