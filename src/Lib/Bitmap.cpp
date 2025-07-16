#include "Bitmap.hpp"

namespace MathLib {
    Bitmap::Bitmap(void) : array(), lastByteSize(8) {
        EmptyBenchmark
    }
    Bitmap::Bitmap(size_t size) : array(), lastByteSize(8) {
        StartBenchmark
        size_t s = size / 8;
        const uint8_t tmp = size % 8;
        if (tmp) {
            s++;
            lastByteSize = tmp;
        }
        array = Array<uint8_t>(s);
        EndBenchmark
    }
    Bitmap::Bitmap(const Sequence<bool>& data) : Bitmap(data.GetSize()) {
        StartBenchmark
        const size_t size = GetSize();
        for (size_t i = 0; i < size; i++)
            if (!Set(i, data.At(i))) Panic("Failed to create bitmap");
        EndBenchmark
    }
    Bitmap::Bitmap(const Sequence<uint8_t>& array) : array(CollectionToArray<uint8_t>(array)), lastByteSize(8) {
        EmptyBenchmark
    }
    Bitmap::Bitmap(const Sequence<uint8_t>& array, uint8_t lastByteSize) : array(CollectionToArray<uint8_t>(array)), lastByteSize(lastByteSize) {
        EmptyBenchmark
    }
    uint8_t Bitmap::GetLastByteSize(void) const {
        StartAndReturnFromBenchmark(lastByteSize);
    }
    uint8_t Bitmap::GetLastByteMask(void) const {
        StartBenchmark
        uint8_t ret = 0;
        for (uint8_t i = 0; i < lastByteSize; i++) ret |= 1 << i;
        ReturnFromBenchmark(ret);
    }
    Array<uint8_t> Bitmap::GetArray(void) const {
        StartBenchmark
        Array<uint8_t> ret = array.GetArray();
        ret.At(ret.GetSize() - 1) &= GetLastByteMask();
        ReturnFromBenchmark(ret);
    }
    bool Bitmap::IsFilledWith(bool value) const {
        StartBenchmark
        const uint8_t val = value ? UINT8_MAX : 0;
        const size_t last = array.GetSize() - 1;
        for (size_t i = 0; i < last; i++)
            if (array.At(i) != val) ReturnFromBenchmark(false);
        ReturnFromBenchmark(array.At(last) == (val & GetLastByteMask()));
    }
    void Bitmap::Fill(bool value) {
        StartBenchmark
        const uint8_t val = value ? UINT8_MAX : 0;
        if (!array.IsEmpty()) {
            const size_t last = array.GetSize() - 1;
            for (size_t i = 0; i < last; i++) array.At(i) = val;
            array.At(last) = val & GetLastByteMask();
        }
        EndBenchmark
    }
    size_t Bitmap::GetSize(void) const {
        StartAndReturnFromBenchmark(array.IsEmpty() ? 0 : array.GetSize() * 8 - 8 + lastByteSize);
    }
    bool Bitmap::At(size_t index) const {
        StartAndReturnFromBenchmark(Get(index).Get("Index out of bounds"));
    }
    bool Bitmap::Add(const bool& value) {
        StartBenchmark
        if (lastByteSize != 8) {
            lastByteSize++;
            ReturnFromBenchmark(Set(GetSize() - 1, value));
        }
        lastByteSize = 1;
        ReturnFromBenchmark(array.Add(value));
    }
    bool Bitmap::Reset(void) {
        StartBenchmark
        if (!array.Reset()) ReturnFromBenchmark(false);
        lastByteSize = 8;
        ReturnFromBenchmark(true);
    }
    bool Bitmap::Flip(size_t index) {
        StartBenchmark
        const uint8_t bit = index % 8;
        const uint8_t mask = 1 << bit;
        const size_t offset = index / 8;
        if (offset >= array.GetSize()) ReturnFromBenchmark(false);
        if (offset + 1 == array.GetSize() && bit >= lastByteSize) ReturnFromBenchmark(false);
        if (array.At(offset) & mask) array.At(offset) &= ~mask;
        else array.At(offset) |= mask;
        ReturnFromBenchmark(true);
    }
    Expected<bool> Bitmap::Get(size_t index) const {
        StartBenchmark
        const size_t offset = index / 8;
        const uint8_t bit = index % 8;
        const uint8_t mask = 1 << bit;
        ReturnFromBenchmark((offset < array.GetSize() && (offset + 1 != array.GetSize() || bit < lastByteSize)) ? array.At(offset) & mask : Expected<bool>());
    }
    bool Bitmap::Set(size_t index, const bool& value) {
        StartBenchmark
        const size_t offset = index / 8;
        const uint8_t bit = index % 8;
        const uint8_t mask = 1 << bit;
        if (offset >= array.GetSize()) ReturnFromBenchmark(false);
        if (offset + 1 == array.GetSize() && bit >= lastByteSize) ReturnFromBenchmark(false);
        if (value) array.At(offset) |= mask;
        else array.At(offset) &= ~mask;
        ReturnFromBenchmark(true);
    }
    uint64_t Bitmap::Read(size_t index, uint8_t bits) const {
        StartBenchmark
        uint64_t ret = 0;
        for (uint8_t i = 0; i < bits; i++) ret |= At(index + i) << i;
        ReturnFromBenchmark(ret);
    }
    bool Bitmap::Save(Writable& file) const {
        StartAndReturnFromBenchmark(file.Write<uint8_t>(lastByteSize) && array.Save(file));
    }
    bool Bitmap::Load(Readable& file) {
        StartAndReturnFromBenchmark(file.Read<uint8_t>(lastByteSize) && IsBetween(lastByteSize, 1, 8) && array.Read(file));
    }
    Bitmap Bitmap::operator~(void) const {
        StartBenchmark
        Bitmap ret = *this;
        const size_t size = ret.GetSize();
        for (size_t i = 0; i < size; i++)
            if (!ret.Set(i, !At(i))) ReturnFromBenchmark(Bitmap());
        ReturnFromBenchmark(ret);
    }
    Bitmap Bitmap::operator^(const Bitmap& other) const {
        StartBenchmark
        Bitmap ret = *this;
        ret ^= other;
        ReturnFromBenchmark(ret);
    }
    Bitmap Bitmap::operator|(const Bitmap& other) const {
        StartBenchmark
        Bitmap ret = *this;
        ret |= other;
        ReturnFromBenchmark(ret);
    }
    Bitmap Bitmap::operator&(const Bitmap& other) const {
        StartBenchmark
        Bitmap ret = *this;
        ret &= other;
        ReturnFromBenchmark(ret);
    }
    Bitmap& Bitmap::operator^=(const Bitmap& other) {
        StartBenchmark
        const size_t size = GetSize();
        const size_t max = Max<size_t>(size, other.GetSize());
        for (size_t i = 0; i < max; i++) {
            if (i < size) {
                if (!Set(i, At(i) ^ other.Get(i).GetOr(false))) Panic("Failed to xor bitmaps");
            }
            else if (!Add(other.Get(i).GetOr(false))) Panic("Failed to xor bitmaps");
        }
        ReturnFromBenchmark(*this);
    }
    Bitmap& Bitmap::operator&=(const Bitmap& other) {
        StartBenchmark
        const size_t size = GetSize();
        const size_t max = Max<size_t>(size, other.GetSize());
        for (size_t i = 0; i < max; i++) {
            if (i < size) {
                if (!Set(i, At(i) & other.Get(i).GetOr(false))) Panic("Failed to and bitmaps");
            }
            else if (!Add(false)) Panic("Failed to and bitmaps");
        }
        ReturnFromBenchmark(*this);
    }
    Bitmap& Bitmap::operator|=(const Bitmap& other) {
        StartBenchmark
        const size_t size = GetSize();
        const size_t max = Max<size_t>(size, other.GetSize());
        for (size_t i = 0; i < max; i++) {
            if (i < size) {
                if (!Set(i, At(i) | other.Get(i).GetOr(false))) Panic("Failed to or bitmaps");
            }
            else if (!Add(other.Get(i).GetOr(false))) Panic("Failed to or bitmaps");
        }
        ReturnFromBenchmark(*this);
    }
    Bitmap MakeBitmap(bool arg) {
        StartBenchmark
        Bitmap ret = Bitmap(1);
        ret.Fill(arg);
        ReturnFromBenchmark(ret);
    }
}