#ifdef __x86_64__
#include "SegmentNotPresent.hpp"
#include "SelectorErrorCode.hpp"
#include <String.hpp>

SegmentNotPresent::SegmentNotPresent(void) : Exception(Type::SegmentNotPresent) {}
MathLib::String SegmentNotPresent::GetPanicMessage(uintptr_t error) const {
    return MathLib::String("Segment without present attribute used\n") + ((SelectorErrorCode*)&error)->ToString() + '\n';
}

#endif