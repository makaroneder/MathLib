#include "ByteArray.hpp"

namespace MathLib {
    ByteArray::ByteArray(void) : array(0) {}
    ByteArray::ByteArray(size_t size) : array(size) {}
    ByteArray::ByteArray(const Sequence<uint8_t>& sequence) : array(CollectionToArray<uint8_t>(sequence)) {}
    ByteArray::ByteArray(ByteDevice& device) : array(device.GetSize()) {
        if (!device.ReadBuffer(array.GetValue(), array.GetSize())) Panic("Failed to read byte array");
    }
    Array<uint8_t> ByteArray::GetArray(void) const {
        return array;
    }
    size_t ByteArray::GetSize(void) const {
        return array.GetSize();
    }
    size_t ByteArray::ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) {
        uint8_t* buff = (uint8_t*)buffer;
        const ssize_t maxSize = array.GetSize() - position;
        if (maxSize <= 0) return 0;
        const size_t s = size < (size_t)maxSize ? size : maxSize;
        for (size_t i = 0; i < s; i++) buff[i] = array.AtUnsafe(position + i);
        return s;
    }
    size_t ByteArray::WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) {
        const uint8_t* buff = (const uint8_t*)buffer;
        const size_t min = Min<size_t>(size, array.GetSize() - position);
        for (size_t i = 0; i < min; i++) array.AtUnsafe(i + position) = buff[i];
        for (size_t i = min; i < size; i++)
            if (!array.Add(buff[i])) return i;
        return size;
    }
    bool ByteArray::Add(const uint8_t& val) {
        return array.Add(val);
    }
    bool ByteArray::Reset(void) {
        return array.Reset();
    }
    uint8_t* ByteArray::GetValue(void) {
        return array.GetValue();
    }
    const uint8_t* ByteArray::GetValue(void) const {
        return array.GetValue();
    }
    bool ByteArray::Save(Writable& file) const {
        if (!file.Write<size_t>(array.GetSize())) return false;
        for (const uint8_t& x : array)
            if (!file.Write<uint8_t>(x)) return false;
        return true;
    }
    bool ByteArray::Load(Readable& file) {
        size_t size = 0;
        if (!file.Read<size_t>(size)) return false;
        array = Array<uint8_t>(size);
        for (uint8_t& x : array)
            if (!file.Read<uint8_t>(x)) return false;
        return true;
    }
    String ByteArray::ToString(const Sequence<char>& padding) const {
        return CollectionToString(padding) + MathLib::ToString<uint8_t>(array);
    }
}