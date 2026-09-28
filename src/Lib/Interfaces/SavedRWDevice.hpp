#ifndef MathLib_Interfaces_SavedRWDevice_H
#define MathLib_Interfaces_SavedRWDevice_H
#include "RWDevice.hpp"

namespace MathLib {
    struct SavedRWDevice : RWDevice {
        SavedRWDevice(RWDevice& base, Writable& save);
        [[nodiscard]] virtual size_t ReadSizedBuffer(void* buffer, size_t size) override;
        [[nodiscard]] virtual size_t WriteSizedBuffer(const void* buffer, size_t size) override;
        [[nodiscard]] virtual bool Skip(size_t size) override;

        private:
        RWDevice& base;
        Writable& save;
    };
}

#endif