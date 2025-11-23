#include "MAC.hpp"
#include "String.hpp"

namespace MathLib {
    size_t MAC::GetSize(void) const {
        return SizeOfArray(buffer);
    }
    bool MAC::Add(const uint8_t&) {
        return false;
    }
    bool MAC::Reset(void) {
        return false;
    }
    uint8_t* MAC::GetValue(void) {
        return buffer;
    }
    const uint8_t* MAC::GetValue(void) const {
        return buffer;
    }
    bool MAC::Save(Writable& file) const {
        return file.WriteBuffer(buffer, SizeOfArray(buffer));
    }
    bool MAC::Load(Readable& file) {
        return file.ReadBuffer(buffer, SizeOfArray(buffer));
    }
    String MAC::ToString(const Sequence<char>& padding) const {
        String ret = CollectionToString(padding);
        for (uint8_t i = 0; i < SizeOfArray(buffer); i++) {
            if (i) ret += ':';
            ret += MathLib::ToString(buffer[i], 16, 2);
        }
        return ret;
    }
}