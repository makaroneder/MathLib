#ifndef MathLib_MBR_H
#define MathLib_MBR_H
#include "MBRHeader.hpp"
#include "../Interval.hpp"
#include "../Interfaces/ByteDevice.hpp"

namespace MathLib {
    struct MBR {
        MBR(ByteDevice& base);
        [[nodiscard]] size_t GetSectorSize(void) const;
        [[nodiscard]] Array<Interval<size_t>> GetPartitionRanges(void) const;

        private:
        ByteDevice& base;
        Array<Interval<size_t>> partitionRanges;
    };
}

#endif