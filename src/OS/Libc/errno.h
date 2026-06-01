#ifndef _ERRNO_H
#define _ERRNO_H
#ifdef __cplusplus
extern "C" {
#endif

#define EEXIST 1
#define EINTR 2
#define EISDIR 3

extern int errno;
#define errno errno

#ifdef __cplusplus
}
#endif
#endif