#ifndef PositionedByteDevice_H
#define PositionedByteDevice_H
#include "ByteDevice.hpp"

struct PositionedByteDevice : ByteDevice {
    /// @brief Sets file position
    /// @param offset Offset to set
    /// @param mode Mode to use when setting position
    /// @return Status
    virtual bool Seek(const ssize_t& offset, const SeekMode& mode) override;
    /// @brief Returns current file position
    /// @return Current file position
    virtual size_t Tell(void) const override;

    protected:
    size_t position;
};

#endif