#ifdef __x86_64__
#include "Stack.hpp"
#include <String.hpp>

MathLib::String StackFrameToString(const StackFrame* frame) {
    MathLib::String ret = "{\n";
    for (; frame; frame = frame->bp) ret += MathLib::String("\t0x") + MathLib::ToString(frame->ip, 16) + '\n';
    return ret + '}';
}

#endif