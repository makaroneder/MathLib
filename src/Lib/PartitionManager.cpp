#include "PartitionManager.hpp"

namespace MathLib {
    PartitionManager::PartitionManager(ByteDevice& base) : base(base) {}
    Array<SubByteDevice*> PartitionManager::GetPartitions(void) {
        const Array<Interval<size_t>> ranges = GetPartitionRanges();
        Array<SubByteDevice*> partitions = Array<SubByteDevice*>(ranges.GetSize());
        for (size_t i = 0; i < partitions.GetSize(); i++) {
            partitions.At(i) = new SubByteDevice(base, ranges.At(i));
            if (partitions.At(i)) continue;
            for (size_t j = 0; j < i; j++) delete partitions.AtUnsafe(j);
            return Array<SubByteDevice*>();
        }
        return partitions;
    }
    size_t PartitionManager::GetSize(void) const {
        return base.GetSize();
    }
    size_t PartitionManager::ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) {
        return base.ReadPositionedSizedBuffer(buffer, size, position);
    }
    size_t PartitionManager::WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) {
        return base.WritePositionedSizedBuffer(buffer, size, position);
    }
}