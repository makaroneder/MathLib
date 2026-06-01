#ifndef MathLib_Interfaces_SubByteDevice_H
#define MathLib_Interfaces_SubByteDevice_H
#include "ByteDevice.hpp"
#include "../Interval.hpp"

namespace MathLib {
    struct SubByteDevice : ByteDevice {
        SubByteDevice(ByteDevice& base, const Interval<size_t>& range);
        [[nodiscard]] virtual size_t GetSize(void) const override;
        [[nodiscard]] virtual size_t ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) override;
        [[nodiscard]] virtual size_t WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) override;

        private:
        ByteDevice& base;
        Interval<size_t> range;
    };
}

#endif