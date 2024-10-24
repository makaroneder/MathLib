#ifdef __x86_64__
#include "Stack.hpp"
#include <String.hpp>

String StackFrameToString(const StackFrame* frame) {
    String ret = "{\n";
    for (; frame; frame = frame->bp) ret += String("\t0x") + ::ToString(frame->ip, 16) + '\n';
    return ret + '}';
}

#endif