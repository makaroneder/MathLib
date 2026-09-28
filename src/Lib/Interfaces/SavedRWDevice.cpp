#include "SavedRWDevice.hpp"

namespace MathLib {
    SavedRWDevice::SavedRWDevice(RWDevice& base, Writable& save) : base(base), save(save) {}
    size_t SavedRWDevice::ReadSizedBuffer(void* buffer, size_t size) {
        const size_t tmp = base.ReadSizedBuffer(buffer, size);
        return Min<size_t>(tmp, save.WriteSizedBuffer(buffer, tmp));
    }
    size_t SavedRWDevice::WriteSizedBuffer(const void* buffer, size_t size) {
        const size_t tmp = base.WriteSizedBuffer(buffer, size);
        return Min<size_t>(tmp, save.WriteSizedBuffer(buffer, tmp));
    }
    bool SavedRWDevice::Skip(size_t size) {
        return DefaultSkip(size);
    }
}