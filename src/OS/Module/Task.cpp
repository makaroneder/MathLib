#include "Task.hpp"
#include "SystemCall.hpp"

size_t GetTask(void) {
    return PerformSystemCall(SystemCall::GetTask);
}
bool Kill(size_t task) {
    return PerformSystemCall<uint64_t>(SystemCall::Exit, task);
}
void Exit(void) {
    (void)Kill(GetTask());
    while (true) {}
}