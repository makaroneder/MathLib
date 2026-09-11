#ifndef Shared_ReadWriteSystemCallData_H
#define Shared_ReadWriteSystemCallData_H
#include <stdint.h>

struct ReadWriteSystemCallData {
    uint64_t file;
    uint64_t address;
    uint64_t size;
    uint64_t position;

    ReadWriteSystemCallData(uint64_t file, uint64_t address, uint64_t size, uint64_t position);
};

#endif