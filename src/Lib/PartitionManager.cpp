#include "PartitionManager.hpp"

namespace MathLib {
    PartitionManager::PartitionManager(ByteDevice& base) : base(base) {
        EmptyBenchmark
    }
    Array<SubByteDevice*> PartitionManager::GetPartitions(void) {
        StartBenchmark
        const Array<Interval<size_t>> ranges = GetPartitionRanges();
        Array<SubByteDevice*> partitions = Array<SubByteDevice*>(ranges.GetSize());
        for (size_t i = 0; i < partitions.GetSize(); i++) {
            partitions.At(i) = new SubByteDevice(base, ranges.At(i));
            if (!partitions.At(i)) ReturnFromBenchmark(Array<SubByteDevice*>());
        }
        ReturnFromBenchmark(partitions);
    }
    size_t PartitionManager::GetSize(void) const {
        StartAndReturnFromBenchmark(base.GetSize());
    }
    size_t PartitionManager::ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) {
        StartAndReturnFromBenchmark(base.ReadPositionedSizedBuffer(buffer, size, position));
    }
    size_t PartitionManager::WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) {
        StartAndReturnFromBenchmark(base.WritePositionedSizedBuffer(buffer, size, position));
    }
}