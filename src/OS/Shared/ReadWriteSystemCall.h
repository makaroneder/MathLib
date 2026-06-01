#ifndef ReadWriteSystemCall_H
#define ReadWriteSystemCall_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ReadWriteSystemCall {
    uint64_t file;
    uint64_t address;
    uint64_t size;
    uint64_t position;
} ReadWriteSystemCall;

#ifdef __cplusplus
}
#endif
#endif