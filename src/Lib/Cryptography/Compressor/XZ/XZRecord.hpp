#ifndef MathLib_Cryptography_Compressor_XZ_Record_H
#define MathLib_Cryptography_Compressor_XZ_Record_H
#include "../../../Interfaces/Saveable.hpp"
#include "../../ReadableCipher.hpp"
#include "XZFlags.hpp"

namespace MathLib {
    struct XZRecord;
    struct XZRecord : Saveable, ReadableCipher, Comparable<XZRecord> {
        size_t unpaddedSize;
        size_t uncompressedSize;

        XZRecord(void);
        XZRecord(size_t unpaddedSize, size_t uncompressedSize);
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;
        [[nodiscard]] Array<uint8_t> Get(const Sequence<uint8_t>& data, XZFlags::CheckType checkType, uint8_t& paddingSize) const;
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
        [[nodiscard]] virtual Array<uint8_t> DecryptReadablePartial(Readable& readable, const CipherKey& key, const Interval<size_t>& range) const override;
        [[nodiscard]] virtual bool Equals(const XZRecord& other) const override;
    };
}

#endif