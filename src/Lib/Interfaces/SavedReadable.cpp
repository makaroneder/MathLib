#include "SavedReadable.hpp"

namespace MathLib {
    SavedReadable::SavedReadable(Readable& readable, Writable& writable) : readable(readable), writable(writable) {}
    size_t SavedReadable::ReadSizedBuffer(void* buffer, size_t size) {
        uint8_t* const buffer8 = (uint8_t*)buffer;
        for (size_t i = 0; i < size; i++)
            if (!readable.Read<uint8_t>(buffer8[i]) || !writable.Write<uint8_t>(buffer8[i])) return i;
        return size;
    }
    bool SavedReadable::Skip(size_t size) {
        return DefaultSkip(size);
    }
}