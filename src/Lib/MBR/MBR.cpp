#include "MBR.hpp"

namespace MathLib {
    MBR::MBR(ByteDevice& base) : base(base) {
        StartBenchmark
        const MBRHeader header = base.Read<MBRHeader>().Get("Failed to read MBR header");
        // TODO: Parse extended partitions
        if (header.IsValid()) {
            for (size_t i = 0; i < 4; i++)
                if (header.entries[i].id != MBRPartitionEntry::ID::Empty && !partitionRanges.Add(Interval<size_t>(header.entries[i].startLBA * GetSectorSize(), (header.entries[i].startLBA + header.entries[i].sectorCount) * GetSectorSize()))) Panic("Failed to add partition range");
        }
        EndBenchmark
    }
    size_t MBR::GetSectorSize(void) const {
        StartBenchmark
        ReturnFromBenchmark(512);
    }
    Array<Interval<size_t>> MBR::GetPartitionRanges(void) const {
        StartBenchmark
        ReturnFromBenchmark(partitionRanges);
    }
}