#ifndef Module_SystemCall_H
#define Module_SystemCall_H
#include <SystemCalls.hpp>

extern "C" [[nodiscard]] uint64_t PerformRawSystemCall(SystemCall systemCall, const void* args);
template <typename T>
[[nodiscard]] uint64_t PerformSystemCall(SystemCall systemCall, const T& args) {
    return PerformRawSystemCall(systemCall, &args);
}
[[nodiscard]] uint64_t PerformSystemCall(SystemCall systemCall);

#endif