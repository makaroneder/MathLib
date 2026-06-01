#ifndef MathLib_PartitionManager_H
#define MathLib_PartitionManager_H
#include "Interval.hpp"
#include "Interfaces/ByteDevice.hpp"
#include "Interfaces/SubByteDevice.hpp"

namespace MathLib {
    struct PartitionManager : ByteDevice {
        PartitionManager(ByteDevice& base);
        [[nodiscard]] virtual Array<Interval<size_t>> GetPartitionRanges(void) const = 0;
        [[nodiscard]] Array<SubByteDevice*> GetPartitions(void);
        [[nodiscard]] virtual size_t GetSize(void) const override;
        [[nodiscard]] virtual size_t ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) override;
        [[nodiscard]] virtual size_t WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) override;

        protected:
        ByteDevice& base;
    };
}

#endif