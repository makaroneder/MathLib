#ifndef Shared_SystemCalls_H
#define Shared_SystemCalls_H
#include <stdint.h>

enum class SystemCall : uint64_t {
    Exit = 0,
    GetTask,
    AllocateMemory,
    FreeMemory,
    ReadFile,
    WriteFile,
    GetSizeOfFile,
    CloseFile,
    OpenFile,
};

#endif