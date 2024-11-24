#ifndef Interfaces_SubByteDevice_H
#define Interfaces_SubByteDevice_H
#include "ByteDevice.hpp"
#include "../Interval.hpp"

namespace MathLib {
    struct SubByteDevice : ByteDevice {
        SubByteDevice(ByteDevice& base, const Interval<size_t>& range);
        /// @brief Returns size
        /// @return Size
        virtual size_t GetSize(void) const override;
        /// @brief Reads data from the buffer
        /// @param buffer Buffer to read data from
        /// @param size Size of the buffer
        /// @param position Position to read from
        /// @return Size of bytes read
        virtual size_t ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) override;
        /// @brief Writes data to the buffer
        /// @param buffer Buffer to write data to
        /// @param size Size of the buffer
        /// @param position Position to write to
        /// @return Size of written bytes
        virtual size_t WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) override;

        private:
        ByteDevice& base;
        Interval<size_t> range;
    };
}

#endif