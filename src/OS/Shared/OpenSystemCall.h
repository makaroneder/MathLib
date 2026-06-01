#ifndef OpenSystemCall_H
#define OpenSystemCall_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenSystemCall {
    const char* path;
    bool create;
} OpenSystemCall;

#ifdef __cplusplus
}
#endif
#endif