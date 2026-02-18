#ifndef MathLib_Cryptography_Compressor_XZ_Filter_H
#define MathLib_Cryptography_Compressor_XZ_Filter_H
#include "../../../Typedefs.hpp"

namespace MathLib {
    struct XZFilter;
    struct XZFilter : Comparable<XZFilter> {
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
        Array<uint8_t> data;
        Type id;

        XZFilter(void);
        XZFilter(Type id, const Array<uint8_t>& data);

        protected:
        [[nodiscard]] virtual bool Equals(const XZFilter& other) const override;
    };
}

#endif