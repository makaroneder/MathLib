#ifndef DirectoryEntryDate_H
#define DirectoryEntryDate_H
#include <stdint.h>

struct DirectoryEntryDate {
    uint8_t yearSince1900;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t timeZoneOffset;

    bool IsValid(void) const;
} __attribute__((packed));

#endif