#include "ISO9660DirectoryEntryDate.hpp"

namespace MathLib {
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