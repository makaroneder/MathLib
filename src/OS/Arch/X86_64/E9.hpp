#ifdef __x86_64__
#ifndef E9_H
#define E9_H
#include <Interfaces/Writable.hpp>

/// @brief E9 logger
struct E9 : MathLib::Writable {
    /// @brief Checks if E9 logger is present
    /// @return Presence of E9 logger
    [[nodiscard]] static bool IsPresent(void);
    /// @brief Writes data to the buffer
    /// @param buffer Buffer to write data to
    /// @param size Size of the buffer
    /// @return Size of written bytes
    [[nodiscard]] virtual size_t WriteSizedBuffer(const void* buffer, size_t size) override;
};

#endif
#endif