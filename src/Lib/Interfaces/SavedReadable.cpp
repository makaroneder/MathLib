#include "SavedReadable.hpp"

namespace MathLib {
    SavedReadable::SavedReadable(Readable& readable, Writable& writable) : readable(readable), writable(writable) {}
    size_t SavedReadable::ReadSizedBuffer(void* buffer, size_t size) {
        const size_t tmp = readable.ReadSizedBuffer(buffer, size);
        return Min<size_t>(tmp, writable.WriteSizedBuffer(buffer, tmp));
    }
    bool SavedReadable::Skip(size_t size) {
        return DefaultSkip(size);
    }
}