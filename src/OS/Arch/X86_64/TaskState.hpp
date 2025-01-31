#ifdef __x86_64__
#ifndef TaskState_H
#define TaskState_H
#include <stdint.h>

struct TaskState {
    uintptr_t ip;
    uintptr_t sp;
} __attribute__((packed));

#endif
#endif