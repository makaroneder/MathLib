#include "Readable.hpp"

namespace MathLib {
    bool Readable::ReadBuffer(void* buffer, size_t size) {
        return ReadSizedBuffer(buffer, size) == size;
    }
    Expected<char> Readable::Getc(void) {
        char chr;
        return Read<char>(chr) ? Expected<char>(chr) : Expected<char>();
    }
    String Readable::ReadUntil(char end) {
        String ret;
        while (true) {
            const Expected<char> chr = Getc();
            if (!chr.HasValue() || chr.Get() == end) break;
            ret += chr.Get();
        }
        return ret;
    }
}