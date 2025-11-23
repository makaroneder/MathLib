#ifndef MathLib_Emulator_H
#define MathLib_Emulator_H
#include "../Interfaces/ByteDevice.hpp"

namespace MathLib {
    struct Emulator : ByteDevice {
        Emulator(void);
        Emulator(const Sequence<uint8_t>& memory);
        [[nodiscard]] virtual bool Run(void) = 0;
        [[nodiscard]] virtual bool Reset(void) = 0;
        /// @brief Returns memory size
        /// @return Memory size
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

        Array<uint8_t> memory;
    };
}

#endif