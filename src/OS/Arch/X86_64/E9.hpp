#ifndef E9_H
#define E9_H
#include <Interfaces/Writeable.hpp>

/// @brief E9 logger
struct E9 : Writeable {
    /// @brief Checks if E9 logger is present
    /// @return Presence of E9 logger
    static bool IsPresent(void);
    /// @brief Writes data to the buffer
    /// @param buffer Buffer to write data to
    /// @param size Size of the buffer
    /// @return Size of written bytes
    virtual size_t WriteSizedBuffer(const void* buffer, const size_t& size) override;
};

#endif