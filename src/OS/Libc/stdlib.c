#include "stdlib.h"
#include "string.h"
#include "SystemCall.h"
#include <SystemCalls.h>

int atexit(void (*func)(void)) {
    // TODO:
    (void)func;
    abort();
    return 0;
}
char* getenv(const char* str) {
    // TODO:
    (void)str;
    abort();
    return NULL;
}
void* malloc(size_t size_) {
    uint64_t size = size_;
    return (void*)PerformSystemCall((uint64_t)AllocateMemorySystemCall, &size);
}
void* calloc(size_t count, size_t elementSize) {
    const size_t size = count * elementSize;
    return memset(malloc(size), 0, size);
}
void* realloc(void* ptr, size_t size) {
    // TODO:
    (void)ptr;
    (void)size;
    abort();
    return NULL;
}
void free(void* ptr) {
    (void)PerformSystemCall((uint64_t)FreeMemorySystemCall, ptr);
}
int atoi(const char* str) {
    // TODO:
    (void)str;
    abort();
    return 0;
}
double atof(const char* str) {
    // TODO:
    (void)str;
    abort();
    return 0;
}
int abs(int x) {
    return x < 0 ? -x : x;
}
long int labs(long int x) {
    return x < 0 ? -x : x;
}
long long int llabs(long long int x) {
    return x < 0 ? -x : x;
}
int system(const char* cmd) {
    // TODO:
    (void)cmd;
    abort();
    return 0;
}