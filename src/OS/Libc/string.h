#ifndef _STRING_H
#define _STRING_H
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* memcpy(void* dst, const void* src, size_t size);
void* memset(void* dst, int value, size_t size);
void* memmove(void* dst, const void* src, size_t size);

size_t strlen(const char* str);
char* strcat(char* dst, const char* src);
char* strchr(const char* str, int target);
char* strrchr(const char* str, int target);
char* strstr(const char* str, const char* target);
char* strcpy(char* dst, const char* src);
char* strncpy(char* dst, const char* src, size_t size);
int strcmp(const char* a, const char* b);
int strncmp(const char* a, const char* b, size_t size);
char* strdup(const char* str);
char* strndup(const char* str, size_t size);

#ifdef __cplusplus
}
#endif
#endif