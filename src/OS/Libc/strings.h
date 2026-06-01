#ifndef _STRINGS_H
#define _STRINGS_H
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int strcasecmp(const char* a, const char* b);
int strncasecmp(const char* a, const char* b, size_t size);

#ifdef __cplusplus
}
#endif
#endif