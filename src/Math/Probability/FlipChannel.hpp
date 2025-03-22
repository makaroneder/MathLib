#ifndef FlipChannel_H
#define FlipChannel_H
#include "Channel.hpp"
#include <Bitmap.hpp>
#include <Interfaces/IdentitySequence.hpp>

template <typename Number, typename Measure>
struct FlipChannel : Channel {
    virtual MathLib::Array<uint8_t> Convert(const MathLib::Sequence<uint8_t>& message) const override {
        MathLib::Bitmap ret = MathLib::Bitmap(message);
        const Measure measure = Measure(MathLib::IdentitySequence<Number>(ret.GetSize()));
        for (size_t i = 0; i < ret.GetSize(); i++)
            if (MathLib::RandomFloat() <= measure.MassFunction(i) && !ret.Flip(i)) return MathLib::Array<uint8_t>();
        return MathLib::CollectionToArray<uint8_t>(ret.GetArray());
    }
};

#endif