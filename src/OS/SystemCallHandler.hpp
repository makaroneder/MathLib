#ifndef SystemCallHandler_H
#define SystemCallHandler_H
#include <stddef.h>

struct SystemCallHandler {
    [[nodiscard]] size_t OnSystemCall(size_t num, void* val1, void* val2, void* val3);
};

#endif