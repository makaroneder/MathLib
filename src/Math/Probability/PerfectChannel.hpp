#ifndef PerfectChannel_H
#define PerfectChannel_H
#include "Channel.hpp"

struct PerfectChannel : Channel {
    virtual MathLib::Array<uint8_t> Convert(const MathLib::Sequence<uint8_t>& message) const override;
};

#endif