#ifndef MathLib_Cryptography_Channel_FlipChannel_H
#define MathLib_Cryptography_Channel_FlipChannel_H
#include "../Cipher.hpp"
#include "../../Bitmap.hpp"
#include "../../Interfaces/Sequence/IdentitySequence.hpp"

namespace MathLib {
    template <typename Number, typename Measure>
    struct FlipChannel : OneWayCipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const override {
            if (!key.IsEmpty()) return Array<uint8_t>();
            Bitmap ret = Bitmap(data);
            const Measure measure = Measure(IdentitySequence<Number>(ret.GetSize()));
            for (size_t i = 0; i < ret.GetSize(); i++)
                if (RandomFloat() <= measure.MassFunction(i) && !ret.Flip(i)) return Array<uint8_t>();
            return CollectionToArray<uint8_t>(ret.GetArray());
        }
    };
}

#endif