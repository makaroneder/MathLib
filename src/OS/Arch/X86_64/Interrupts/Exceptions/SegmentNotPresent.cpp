#ifdef __x86_64__
#include "SegmentNotPresent.hpp"
#include <String.hpp>

SegmentNotPresent::SegmentNotPresent(void) : Exception(Type::SegmentNotPresent) {}
String SegmentNotPresent::GetPanicMessage(uintptr_t error) const {
    return String("Segment without present attribute used\nSegment selector: 0x") + ToString(error, 16) + '\n';
}

#endif