#ifndef _SYS_TYPES_H
#define _SYS_TYPES_H
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int pid_t;
typedef struct flock {
    int l_whence;
    int l_start;
    int l_len;
    pid_t l_pid;
    int l_type;
} flock;

#ifdef __cplusplus
}
#endif
#endif