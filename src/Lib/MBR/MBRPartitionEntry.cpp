#include "MBRPartitionEntry.hpp"

namespace MathLib {
    MBRPartitionEntry::MBRPartitionEntry(void) : MBRPartitionEntry(Interval<uint32_t>(), ID::Empty) {
        EmptyBenchmark
    }
    MBRPartitionEntry::MBRPartitionEntry(const Interval<uint32_t>& lba, ID id, Type type, uint8_t heads, uint8_t sectors) : type(type), startCHS(MBRPartitionEntryCHS(lba.GetMin(), heads, sectors)), id(id), endCHS(MBRPartitionEntryCHS(lba.GetMax(), heads, sectors)), startLBA(lba.GetMin()), sectorCount(lba.GetSize()) {
        EmptyBenchmark
    }
    bool MBRPartitionEntry::IsValid(void) const {
        StartAndReturnFromBenchmark((type == Type::Normal || type == Type::Active) && !(id == ID::Empty && type == Type::Active));
    }
    Interval<size_t> MBRPartitionEntry::GetInterval(void) const {
        StartAndReturnFromBenchmark(Interval<size_t>((startLBA + 1) * 512, (startLBA + sectorCount + 1) * 512));
    }
}