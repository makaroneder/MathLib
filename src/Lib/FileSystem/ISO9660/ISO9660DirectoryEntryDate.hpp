#ifndef MathLib_FileSystem_ISO9660_DirectoryEntryDate_H
#define MathLib_FileSystem_ISO9660_DirectoryEntryDate_H
#include <stdint.h>

namespace MathLib {
    struct ISO9660DirectoryEntryDate {
        uint8_t yearSince1900;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        uint8_t timeZoneOffset;

        ISO9660DirectoryEntryDate(void);
        [[nodiscard]] bool IsValid(void) const;
    } __attribute__((packed));
}

#endif