#ifndef MainData_H
#define MainData_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MainData {
    uint64_t in;
    uint64_t out;
    char** argv;
    int argc;
} MainData;

#ifdef __cplusplus
}
#endif
#endif