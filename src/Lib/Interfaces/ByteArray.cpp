#include "ByteArray.hpp"

namespace MathLib {
    ByteArray::ByteArray(void) : array(0) {
        EmptyBenchmark
    }
    ByteArray::ByteArray(size_t size) : array(size) {
        EmptyBenchmark
    }
    ByteArray::ByteArray(const Sequence<uint8_t>& sequence) : array(CollectionToArray<uint8_t>(sequence)) {
        EmptyBenchmark
    }
    ByteArray::ByteArray(ByteDevice& device) : array(device.GetSize()) {
        StartBenchmark
        if (!device.ReadBuffer((uint8_t*)array.GetValue(), array.GetSize())) MathLib::Panic("Failed to read byte array");
        EndBenchmark
    }
    Array<uint8_t> ByteArray::GetArray(void) const {
        StartAndReturnFromBenchmark(array);
    }
    size_t ByteArray::GetSize(void) const {
        StartAndReturnFromBenchmark(array.GetSize());
    }
    size_t ByteArray::ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) {
        StartBenchmark
        uint8_t* buff = (uint8_t*)buffer;
        const ssize_t maxSize = array.GetSize() - position;
        if (maxSize <= 0) ReturnFromBenchmark(0);
        const size_t s = size < (size_t)maxSize ? size : maxSize;
        for (size_t i = 0; i < s; i++) buff[i] = array.At(position + i);
        ReturnFromBenchmark(s);
    }
    size_t ByteArray::WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) {
        StartBenchmark
        const uint8_t* buff = (const uint8_t*)buffer;
        const size_t min = Min<size_t>(size, array.GetSize() - position);
        for (size_t i = 0; i < min; i++) array.At(i + position) = buff[i];
        for (size_t i = min; i < size; i++)
            if (!array.Add(buff[i])) ReturnFromBenchmark(i);
        ReturnFromBenchmark(size);
    }
    uint8_t ByteArray::At(size_t index) const {
        StartAndReturnFromBenchmark(array.At(index));
    }
    uint8_t& ByteArray::At(size_t index) {
        StartAndReturnFromBenchmark(array.At(index));
    }
    bool ByteArray::Add(const uint8_t& val) {
        StartAndReturnFromBenchmark(array.Add(val));
    }
    bool ByteArray::Reset(void) {
        StartAndReturnFromBenchmark(array.Reset());
    }
    const uint8_t* ByteArray::GetValue(void) const {
        StartAndReturnFromBenchmark(array.GetValue());
    }
    bool ByteArray::Save(Writable& file) const {
        StartBenchmark
        if (!file.Write<size_t>(array.GetSize())) ReturnFromBenchmark(false);
        for (const uint8_t& x : array)
            if (!file.Write<uint8_t>(x)) ReturnFromBenchmark(false);
        ReturnFromBenchmark(true);
    }
    bool ByteArray::Load(Readable& file) {
        StartBenchmark
        size_t size = 0;
        if (!file.Read<size_t>(size)) ReturnFromBenchmark(false);
        array = Array<uint8_t>(size);
        for (uint8_t& x : array)
            if (!file.Read<uint8_t>(x)) ReturnFromBenchmark(false);
        ReturnFromBenchmark(true);
    }
    String ByteArray::ToString(const Sequence<char>& padding) const {
        return MathLib::CollectionToString(padding) + MathLib::ToString<uint8_t>(array);
    }
}