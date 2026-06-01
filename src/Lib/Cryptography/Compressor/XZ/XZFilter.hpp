#ifndef MathLib_Cryptography_Compressor_XZ_Filter_H
#define MathLib_Cryptography_Compressor_XZ_Filter_H
#include "../../Cipher.hpp"
#include "../../../Interfaces/Saveable.hpp"

namespace MathLib {
    struct XZFilter;
    struct XZFilter : Cipher, Saveable, Comparable<XZFilter> {
        enum class Type : size_t {
            Delta = 0x03,
            BranchCallJumpX86,
            BranchCallJumpPowerPC,
            BranchCallJumpIA64,
            BranchCallJumpArm,
            BranchCallJumpArmThumb,
            BranchCallJumpSPARC,
            BranchCallJumpARM64,
            BranchCallJumpRISCV,
            LZMA2 = 0x21,
        };
        Array<uint8_t> key;
        Type id;

        XZFilter(void);
        XZFilter(Type id, const Array<uint8_t>& key);
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
        [[nodiscard]] virtual Array<uint8_t> DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const override;
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;

        protected:
        [[nodiscard]] virtual bool Equals(const XZFilter& other) const override;
    };
}

#endif