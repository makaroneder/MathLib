#include "MBR.hpp"

MBR::MBR(ByteDevice& base) : base(base) {
    const MBRHeader header = base.Read<MBRHeader>().Get("Failed to read MBR header");
    // TODO: Parse extended partitions
    if (header.IsValid()) {
        for (size_t i = 0; i < 4; i++)
            if (header.entries[i].id != MBRPartitionEntry::ID::Empty && !partitionRanges.Add(Interval<size_t>(header.entries[i].startLBA * GetSectorSize(), (header.entries[i].startLBA + header.entries[i].sectorCount) * GetSectorSize()))) Panic("Failed to add partition range");
    }
}
size_t MBR::GetSectorSize(void) const {
    return 512;
}
Array<Interval<size_t>> MBR::GetPartitionRanges(void) const {
    return partitionRanges;
}