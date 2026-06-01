#include "exit.h"
#include "unistd.h"
#include "SystemCall.h"
#include <SystemCalls.h>

#ifdef __cplusplus
extern "C" {
#endif

uintptr_t __stack_chk_guard = 0x2137699642012345;
void __stack_chk_fail(void) {
    abort();
}
void _fini(void);

#ifdef __cplusplus
}
#endif

void exit(int status) {
    (void)status;
    _fini();
    pid_t pid = getpid();
    (void)PerformSystemCall((uint64_t)ExitSystemCall, &pid);
    while (true) {}
}
void abort(void) {
    exit(1);
}