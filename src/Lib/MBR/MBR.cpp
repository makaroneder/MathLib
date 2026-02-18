#include "MBR.hpp"

namespace MathLib {
    MBR::MBR(ByteDevice& base) : PartitionManager(base) {
        MBRHeader header;
        // TODO: Parse extended partitions
        if (ReadPositioned<MBRHeader>(header, 0) && header.IsValid()) {
            for (uint8_t i = 0; i < 4; i++) {
                if (header.entries[i].id != MBRPartitionEntry::ID::Empty && !partitions.Add(header.entries[i])) Panic("Failed to add partition range");
            }
        }
    }
    size_t MBR::GetSectorSize(void) const {
        return 512;
    }
    Array<MBRPartitionEntry> MBR::GetPartitionEntries(void) const {
        return partitions;
    }
    Array<Interval<size_t>> MBR::GetPartitionRanges(void) const {
        Array<Interval<size_t>> ret = Array<Interval<size_t>>(partitions.GetSize());
        for (size_t i = 0; i < ret.GetSize(); i++) ret.At(i) = partitions.At(i).GetInterval();
        return ret;
    }
}