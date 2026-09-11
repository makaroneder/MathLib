#ifndef SystemCall_H
#define SystemCall_H
#include <SystemCalls.hpp>

[[nodiscard]] uint64_t OnSystemCall(SystemCall systemCall, void* args);

#endif