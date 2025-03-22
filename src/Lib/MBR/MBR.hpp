#ifndef MathLib_MBR_H
#define MathLib_MBR_H
#include "MBRHeader.hpp"
#include "../PartitionManager.hpp"

namespace MathLib {
    struct MBR : PartitionManager {
        MBR(ByteDevice& base);
        [[nodiscard]] size_t GetSectorSize(void) const;
        [[nodiscard]] Array<MBRPartitionEntry> GetPartitionEntries(void) const;
        [[nodiscard]] virtual Array<Interval<size_t>> GetPartitionRanges(void) const override;

        private:
        Array<MBRPartitionEntry> partitions;
    };
}

#endif