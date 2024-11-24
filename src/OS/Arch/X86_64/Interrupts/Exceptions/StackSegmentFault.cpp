#ifdef __x86_64__
#include "StackSegmentFault.hpp"
#include "SelectorErrorCode.hpp"
#include <String.hpp>

StackSegmentFault::StackSegmentFault(void) : Exception(Type::StackSegmentFault) {}
MathLib::String StackSegmentFault::GetPanicMessage(uintptr_t error) const {
    return MathLib::String("Stack segment error detected\n") + ((SelectorErrorCode*)&error)->ToString() + '\n';
}

#endif