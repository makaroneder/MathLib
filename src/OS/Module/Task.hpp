#ifndef Module_Task_H
#define Module_Task_H
#include <stddef.h>

[[nodiscard]] size_t GetTask(void);
[[nodiscard]] bool Kill(size_t task);
[[noreturn]] void Exit(void);

#endif