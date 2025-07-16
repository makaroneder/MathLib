#ifndef MathLib_Emulator_Register_H
#define MathLib_Emulator_Register_H
#include "../Interfaces/Printable.hpp"

namespace MathLib {
    struct Register : Printable {
        Register(uint64_t value = 0);
        [[nodiscard]] uint32_t Get32(bool upper) const;
        [[nodiscard]] uint16_t Get16(bool upper) const;
        [[nodiscard]] uint8_t Get8(bool upper) const;
        [[nodiscard]] uint8_t Get4(bool upper) const;
        void Set32(uint32_t val, bool upper);
        void Set16(uint16_t val, bool upper);
        void Set8(uint8_t val, bool upper);
        void Clear4(bool upper);
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
        [[nodiscard]] bool operator==(const Register& other) const;
        [[nodiscard]] bool operator!=(const Register& other) const;

        uint64_t value;
    };
}

#endif