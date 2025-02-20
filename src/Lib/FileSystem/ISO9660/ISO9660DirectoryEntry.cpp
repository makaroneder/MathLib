#include "ISO9660DirectoryEntry.hpp"

namespace MathLib {
    bool ISO9660DirectoryEntry::IsValid(void) const {
        StartAndReturnFromBenchmark(length && date.IsValid());
    }
    String ISO9660DirectoryEntry::GetName(void) const {
        StartBenchmark
        String ret;
        for (uint8_t i = 0; i < nameLength && name[i] != ';'; i++) ret += name[i];
        ReturnFromBenchmark(ret);
    }
}