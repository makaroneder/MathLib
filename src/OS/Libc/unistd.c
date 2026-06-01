#include "exit.h"
#include "unistd.h"
#include "SystemCall.h"
#include <SystemCalls.h>

int execv(const char*, char* const[]) {
    // TODO:
    abort();
    return 0;
}
int execve(const char*, char* const[], char* const[]) {
    // TODO:
    abort();
    return 0;
}
int execvp(const char*, char* const[]) {
    // TODO:
    abort();
    return 0;
}
pid_t fork(void) {
    // TODO:
    abort();
    return 0;
}
pid_t getpid(void) {
    return PerformSystemCall((uint64_t)GetTaskSystemCall, NULL);
}