#ifndef MathLib_Cryptography_Compressor_XZ_Index_H
#define MathLib_Cryptography_Compressor_XZ_Index_H
#include "../../../Interfaces/Saveable.hpp"
#include "XZRecord.hpp"

namespace MathLib {
    struct XZIndex : Saveable {
        Array<XZRecord> records;

        XZIndex(void);
        XZIndex(const Array<XZRecord>& records);
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;
    };
}

#endif