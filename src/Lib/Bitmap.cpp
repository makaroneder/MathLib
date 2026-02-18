#include "Bitmap.hpp"

namespace MathLib {
    Bitmap::Bitmap(void) : array(), lastByteSize(8) {}
    Bitmap::Bitmap(size_t size) : array(), lastByteSize(8) {
        size_t s = size / 8;
        const uint8_t tmp = size % 8;
        if (tmp) {
            s++;
            lastByteSize = tmp;
        }
        array = Array<uint8_t>(s);
    }
    Bitmap::Bitmap(const Sequence<bool>& data) : Bitmap(data.GetSize()) {
        const size_t size = GetSize();
        for (size_t i = 0; i < size; i++)
            if (!SetUnsafe(i, data.AtUnsafe(i))) Panic("Failed to create bitmap");
    }
    Bitmap::Bitmap(const Sequence<uint8_t>& array) : array(CollectionToArray<uint8_t>(array)), lastByteSize(8) {}
    Bitmap::Bitmap(const Sequence<uint8_t>& array, uint8_t lastByteSize) : array(CollectionToArray<uint8_t>(array)), lastByteSize(lastByteSize) {}
    uint8_t Bitmap::GetLastByteSize(void) const {
        return lastByteSize;
    }
    uint8_t Bitmap::GetLastByteMask(void) const {
        uint8_t ret = 0;
        for (uint8_t i = 0; i < lastByteSize; i++) ret |= 1 << i;
        return ret;
    }
    Array<uint8_t> Bitmap::GetArray(void) const {
        Array<uint8_t> ret = array.GetArray();
        ret.AtUnsafe(ret.GetSize() - 1) &= GetLastByteMask();
        return ret;
    }
    bool Bitmap::IsFilledWith(bool value) const {
        const uint8_t val = value ? UINT8_MAX : 0;
        const size_t last = array.GetSize() - 1;
        for (size_t i = 0; i < last; i++)
            if (array.At(i) != val) return false;
        return array.At(last) == (val & GetLastByteMask());
    }
    void Bitmap::Fill(bool value) {
        const uint8_t val = value ? UINT8_MAX : 0;
        const size_t size = array.GetSize();
        if (!size) return;
        const size_t last = size - 1;
        for (size_t i = 0; i < last; i++) array.AtUnsafe(i) = val;
        array.AtUnsafe(last) = val & GetLastByteMask();
    }
    size_t Bitmap::GetSize(void) const {
        const size_t size = array.GetSize();
        return size ? size * 8 - 8 + lastByteSize : 0;
    }
    bool Bitmap::AtUnsafe(size_t index) const {
        return Get(index).Get("Index out of bounds");
    }
    bool Bitmap::Add(const bool& value) {
        if (lastByteSize != 8) {
            lastByteSize++;
            return SetUnsafe(GetSize() - 1, value);
        }
        lastByteSize = 1;
        return array.Add(value);
    }
    bool Bitmap::Reset(void) {
        if (!array.Reset()) return false;
        lastByteSize = 8;
        return true;
    }
    bool Bitmap::Flip(size_t index) {
        const uint8_t bit = index % 8;
        const uint8_t mask = 1 << bit;
        const size_t offset = index / 8;
        const size_t size = array.GetSize();
        if (offset >= size || (offset + 1 == size && bit >= lastByteSize)) return false;
        if (array.At(offset) & mask) array.At(offset) &= ~mask;
        else array.At(offset) |= mask;
        return true;
    }
    Expected<bool> Bitmap::Get(size_t index) const {
        const size_t offset = index / 8;
        const uint8_t bit = index % 8;
        const uint8_t mask = 1 << bit;
        const size_t size = array.GetSize();
        return (offset < size && (offset + 1 != size || bit < lastByteSize)) ? array.AtUnsafe(offset) & mask : Expected<bool>();
    }
    bool Bitmap::SetUnsafe(size_t index, const bool& value) {
        const size_t offset = index / 8;
        const uint8_t bit = index % 8;
        const uint8_t mask = 1 << bit;
        const size_t size = array.GetSize();
        if (offset >= size || (offset + 1 == size && bit >= lastByteSize)) return false;
        if (value) array.At(offset) |= mask;
        else array.At(offset) &= ~mask;
        return true;
    }
    uint64_t Bitmap::Read(size_t index, uint8_t bits) const {
        uint64_t ret = 0;
        for (uint8_t i = 0; i < bits; i++) ret |= At(index + i) << i;
        return ret;
    }
    bool Bitmap::Save(Writable& file) const {
        return file.Write<uint8_t>(lastByteSize) && array.Save(file);
    }
    bool Bitmap::Load(Readable& file) {
        return file.Read<uint8_t>(lastByteSize) && IsBetween(lastByteSize, 1, 8) && array.Read(file);
    }
    Bitmap Bitmap::operator~(void) const {
        const size_t size = GetSize();
        Bitmap ret = *this;
        for (size_t i = 0; i < size; i++)
            if (!ret.SetUnsafe(i, !AtUnsafe(i))) return Bitmap();
        return ret;
    }
    Bitmap Bitmap::operator^(const Bitmap& other) const {
        Bitmap ret = *this;
        ret ^= other;
        return ret;
    }
    Bitmap Bitmap::operator|(const Bitmap& other) const {
        Bitmap ret = *this;
        ret |= other;
        return ret;
    }
    Bitmap Bitmap::operator&(const Bitmap& other) const {
        Bitmap ret = *this;
        ret &= other;
        return ret;
    }
    Bitmap& Bitmap::operator^=(const Bitmap& other) {
        const size_t size = GetSize();
        const size_t max = Max<size_t>(size, other.GetSize());
        for (size_t i = 0; i < max; i++) {
            if (i < size) {
                if (!Set(i, At(i) ^ other.Get(i).GetOr(false))) Panic("Failed to xor bitmaps");
            }
            else if (!Add(other.Get(i).GetOr(false))) Panic("Failed to xor bitmaps");
        }
        return *this;
    }
    Bitmap& Bitmap::operator&=(const Bitmap& other) {
        const size_t size = GetSize();
        const size_t max = Max<size_t>(size, other.GetSize());
        for (size_t i = 0; i < max; i++) {
            if (i < size) {
                if (!Set(i, At(i) & other.Get(i).GetOr(false))) Panic("Failed to and bitmaps");
            }
            else if (!Add(false)) Panic("Failed to and bitmaps");
        }
        return *this;
    }
    Bitmap& Bitmap::operator|=(const Bitmap& other) {
        const size_t size = GetSize();
        const size_t max = Max<size_t>(size, other.GetSize());
        for (size_t i = 0; i < max; i++) {
            if (i < size) {
                if (!Set(i, At(i) | other.Get(i).GetOr(false))) Panic("Failed to or bitmaps");
            }
            else if (!Add(other.Get(i).GetOr(false))) Panic("Failed to or bitmaps");
        }
        return *this;
    }
    Bitmap MakeBitmap(bool arg) {
        Bitmap ret = Bitmap(1);
        ret.Fill(arg);
        return ret;
    }
}