#include "CPU.hpp"

uintptr_t GetFlags(void) {
    uintptr_t ret = 0;
    asm volatile (
        "pushfq\n"
        "popq %0" : "=r"(ret)
    );
    return ret;
}
void SetFlags(uintptr_t flags) {
    asm volatile (
        "pushq %0\n"
        "popfq" :: "r"(flags) : "cc"
    );
}