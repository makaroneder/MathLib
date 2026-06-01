#ifndef SystemCall_H
#define SystemCall_H
#include <SystemCalls.h>
#include <stdint.h>

[[nodiscard]] uint64_t OnSystemCall(SystemCall num, void* args);

#endif