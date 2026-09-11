#include "SystemCall.hpp"

uint64_t PerformSystemCall(SystemCall systemCall) {
    return PerformRawSystemCall(systemCall, nullptr);
}