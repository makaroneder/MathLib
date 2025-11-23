#include "ByteDevice.hpp"

namespace MathLib {
    ByteDevice::ByteDevice(void) : position(0) {}
    size_t ByteDevice::ReadSizedBuffer(void* buffer, size_t size) {
        const size_t s = ReadPositionedSizedBuffer(buffer, size, position);
        position += s;
        return s;
    }
    size_t ByteDevice::WriteSizedBuffer(const void* buffer, size_t size) {
        const size_t s = WritePositionedSizedBuffer(buffer, size, position);
        position += s;
        return s;
    }
    bool ByteDevice::Skip(size_t size) {
        position += size;
        return true;
    }
    bool ByteDevice::ReadPositionedBuffer(void* buffer, size_t size, size_t position) {
        return ReadPositionedSizedBuffer(buffer, size, position) == size;
    }
    bool ByteDevice::WritePositionedBuffer(const void* buffer, size_t size, size_t position) {
        return WritePositionedSizedBuffer(buffer, size, position) == size;
    }
    bool ByteDevice::Seek(ssize_t offset, SeekMode mode) {
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
                const size_t size = GetSize();
                if (offset < 0 || size < (size_t)offset) return false;
                position = size - offset;
                break;
            }
            default: return false;
        }
        return true;
    }
    size_t ByteDevice::Tell(void) const {
        return position;
    }
    size_t ByteDevice::GetSizeLeft(void) const {
        return GetSize() - position;
    }
}