#ifdef __x86_64__
#ifndef Stack_H
#define Stack_H
#include <Typedefs.hpp>

struct StackFrame {
    StackFrame* bp;
    uintptr_t ip;
} __attribute__((packed));
String StackFrameToString(const StackFrame* frame);

#endif
#endif