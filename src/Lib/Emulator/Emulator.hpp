#ifndef MathLib_Emulator_H
#define MathLib_Emulator_H
#include "../Interfaces/ByteDevice.hpp"

namespace MathLib {
    struct Emulator : ByteDevice {
        Emulator(void);
        Emulator(const Sequence<uint8_t>& memory);
        [[nodiscard]] virtual bool Run(void) = 0;
        [[nodiscard]] virtual bool Reset(void) = 0;
        [[nodiscard]] virtual size_t GetSize(void) const override;
        [[nodiscard]] virtual size_t ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) override;
        [[nodiscard]] virtual size_t WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) override;

        Array<uint8_t> memory;
    };
}

#endif