#include "PositionedByteDevice.hpp"

bool PositionedByteDevice::Seek(const ssize_t& offset, const SeekMode& mode) {
    switch (mode) {
        case SeekMode::Set: {
            if (offset < 0) return false;
            position = offset;
            break;
        }
        case SeekMode::Current: {
            position += offset;
            break;
        }
        case SeekMode::End: {
            if (offset < 0) return false;
            position = GetSize() - offset;
            break;
        }
        default: return false;
    }
    return true;
}
size_t PositionedByteDevice::Tell(void) const {
    return position;
}