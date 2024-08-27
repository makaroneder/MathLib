#ifndef MBR_H
#define MBR_H
#include "MBRHeader.hpp"
#include "../Interval.hpp"
#include "../Interfaces/ByteDevice.hpp"

struct MBR {
    MBR(ByteDevice& base);
    size_t GetSectorSize(void) const;
    Array<Interval<size_t>> GetPartitionRanges(void) const;

    private:
    ByteDevice& base;
    Array<Interval<size_t>> partitionRanges;
};

#endif