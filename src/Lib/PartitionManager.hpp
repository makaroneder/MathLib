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
        /// @brief Returns size
        /// @return Size
        [[nodiscard]] virtual size_t GetSize(void) const override;
        /// @brief Reads data from the buffer
        /// @param buffer Buffer to read data from
        /// @param size Size of the buffer
        /// @param position Position to read from
        /// @return Size of bytes read
        [[nodiscard]] virtual size_t ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) override;
        /// @brief Writes data to the buffer
        /// @param buffer Buffer to write data to
        /// @param size Size of the buffer
        /// @param position Position to write to
        /// @return Size of written bytes
        [[nodiscard]] virtual size_t WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) override;

        protected:
        ByteDevice& base;
    };
}

#endif