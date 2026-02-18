#include "ISO9660DirectoryEntryDate.hpp"

namespace MathLib {
    ISO9660DirectoryEntryDate::ISO9660DirectoryEntryDate(void) : yearSince1900(0), month(0), day(0), hour(0), minute(0), second(0), timeZoneOffset(0) {}
    bool ISO9660DirectoryEntryDate::IsValid(void) const {
        return !(
            month > 12 || !month ||
            day > 31 || !day ||
            hour > 23 ||
            minute > 59 ||
            second > 59 ||
            timeZoneOffset > 100
        );
    }
}