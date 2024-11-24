#include "SubByteDevice.hpp"

namespace MathLib {
    SubByteDevice::SubByteDevice(ByteDevice& base, const Interval<size_t>& range) : base(base), range(range) {}
    size_t SubByteDevice::GetSize(void) const {
        return range.GetSize();
    }
    size_t SubByteDevice::ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) {
        return base.ReadPositionedSizedBuffer(buffer, position + size >= GetSize() ? GetSize() - position : size, position + range.GetMin());
    }
    size_t SubByteDevice::WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) {
        return base.WritePositionedSizedBuffer(buffer, position + size >= GetSize() ? GetSize() - position : size, position + range.GetMin());
    }
}