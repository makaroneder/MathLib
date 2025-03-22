#include "PerfectChannel.hpp"

MathLib::Array<uint8_t> PerfectChannel::Convert(const MathLib::Sequence<uint8_t>& message) const {
    return MathLib::CollectionToArray<uint8_t>(message);
}