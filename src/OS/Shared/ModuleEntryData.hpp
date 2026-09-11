#ifndef Shared_ModuleEntryData_H
#define Shared_ModuleEntryData_H
#include <stdint.h>

struct ModuleEntryData {
    uint64_t inputFile;
    uint64_t outputFile;

    ModuleEntryData(uint64_t inputFile, uint64_t outputFile);
};

#endif