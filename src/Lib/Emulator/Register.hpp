#ifndef Emulator_Register_H
#define Emulator_Register_H
#include "../Printable.hpp"

struct Register : Printable {
    Register(uint64_t val = 0);
    uint32_t Get32(bool upper) const;
    uint16_t Get16(bool upper) const;
    uint8_t Get8(bool upper) const;
    void Set32(uint32_t val, bool upper);
    void Set16(uint16_t val, bool upper);
    void Set8(uint8_t val, bool upper);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual String ToString(String padding = "") const override;

    uint64_t value;
};

#endif