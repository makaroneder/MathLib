#ifndef _SYS_STAT_H
#define _SYS_STAT_H
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef size_t mode_t;

int mkdir(const char* path, mode_t mode);

#ifdef __cplusplus
}
#endif
#endif