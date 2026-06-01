#ifndef MathLib_Interfaces_SectorByteDevice_H
#define MathLib_Interfaces_SectorByteDevice_H
#include "ByteDevice.hpp"

namespace MathLib {
    struct SectorByteDevice : ByteDevice {
        [[nodiscard]] virtual size_t GetSectorSize(void) const = 0;
        [[nodiscard]] virtual bool ReadSectors(size_t sector, void* buff, size_t count) = 0;
        [[nodiscard]] virtual bool WriteSectors(size_t sector, const void* buff, size_t count) = 0;
        [[nodiscard]] virtual size_t ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) override;
        [[nodiscard]] virtual size_t WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) override;
    };
}

#endif