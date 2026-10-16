#include "CountedReadable.hpp"

namespace MathLib {
    CountedReadable::CountedReadable(Readable& base) : base(base), read(0) {}
    size_t CountedReadable::ReadSizedBuffer(void* buffer, size_t size) {
        const size_t ret = base.ReadSizedBuffer(buffer, size);
        read += ret;
        return ret;
    }
    bool CountedReadable::Skip(size_t size) {
        if (!base.Skip(size)) return false;
        read += size;
        return true;
    }
    size_t CountedReadable::GetReadBytes(void) const {
        return read;
    }
    void CountedReadable::ResetReadBytes(void) {
        read = 0;
    }
}