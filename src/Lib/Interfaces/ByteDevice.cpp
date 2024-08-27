#include "ByteDevice.hpp"

ByteDevice::ByteDevice(void) : position(0) {}
size_t ByteDevice::ReadSizedBuffer(void* buffer, const size_t& size) {
    const size_t s = ReadPositionedSizedBuffer(buffer, size, position);
    position += s;
    return s;
}
size_t ByteDevice::WriteSizedBuffer(const void* buffer, const size_t& size) {
    const size_t s = WritePositionedSizedBuffer(buffer, size, position);
    position += s;
    return s;
}
bool ByteDevice::ReadPositionedBuffer(void* buffer, const size_t& size, const size_t& position) {
    return ReadPositionedSizedBuffer(buffer, size, position) == size;
}
bool ByteDevice::WritePositionedBuffer(const void* buffer, const size_t& size, const size_t& position) {
    return WritePositionedSizedBuffer(buffer, size, position) == size;
}
bool ByteDevice::Seek(const ssize_t& offset, const SeekMode& mode) {
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
            if (offset < 0 || GetSize() < (size_t)offset) return false;
            position = GetSize() - offset;
            break;
        }
        default: return false;
    }
    return true;
}
size_t ByteDevice::Tell(void) const {
    return position;
}