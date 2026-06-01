#ifndef _SYSTEMCALL_H
#define _SYSTEMCALL_H
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint64_t PerformSystemCall(uint64_t num, void* args);

#ifdef __cplusplus
}
#endif
#endif