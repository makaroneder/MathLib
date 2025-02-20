#include "ByteDevice.hpp"

namespace MathLib {
    ByteDevice::ByteDevice(void) : position(0) {
        EmptyBenchmark
    }
    size_t ByteDevice::ReadSizedBuffer(void* buffer, size_t size) {
        StartBenchmark
        const size_t s = ReadPositionedSizedBuffer(buffer, size, position);
        position += s;
        ReturnFromBenchmark(s);
    }
    size_t ByteDevice::WriteSizedBuffer(const void* buffer, size_t size) {
        StartBenchmark
        const size_t s = WritePositionedSizedBuffer(buffer, size, position);
        position += s;
        ReturnFromBenchmark(s);
    }
    bool ByteDevice::ReadPositionedBuffer(void* buffer, size_t size, size_t position) {
        StartAndReturnFromBenchmark(ReadPositionedSizedBuffer(buffer, size, position) == size);
    }
    bool ByteDevice::WritePositionedBuffer(const void* buffer, size_t size, size_t position) {
        StartAndReturnFromBenchmark(WritePositionedSizedBuffer(buffer, size, position) == size);
    }
    bool ByteDevice::Seek(ssize_t offset, SeekMode mode) {
        StartBenchmark
        switch (mode) {
            case SeekMode::Set: {
                if (offset < 0) ReturnFromBenchmark(false);
                position = offset;
                break;
            }
            case SeekMode::Current: {
                position += offset;
                break;
            }
            case SeekMode::End: {
                if (offset < 0 || GetSize() < (size_t)offset) ReturnFromBenchmark(false);
                position = GetSize() - offset;
                break;
            }
            default: ReturnFromBenchmark(false);
        }
        ReturnFromBenchmark(true);
    }
    size_t ByteDevice::Tell(void) const {
        StartAndReturnFromBenchmark(position);
    }
}