#ifndef _STDLIB_H
#define _STDLIB_H
#include "exit.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int atexit(void (*func)(void));
char* getenv(const char* str);
void* malloc(size_t size);
void* calloc(size_t count, size_t elementSize);
void* realloc(void* ptr, size_t size);
void free(void* ptr);
int atoi(const char* str);
double atof(const char* str);
int abs(int x);
long int labs(long int x);
long long int llabs(long long int x);
int system(const char* cmd);

#ifdef __cplusplus
}
#endif
#endif