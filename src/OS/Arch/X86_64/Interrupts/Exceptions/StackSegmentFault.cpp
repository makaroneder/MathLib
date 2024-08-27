#include "StackSegmentFault.hpp"
#include <String.hpp>

StackSegmentFault::StackSegmentFault(void) : Exception(Type::StackSegmentFault) {}
String StackSegmentFault::GetPanicMessage(uintptr_t error) const {
    return String("Stack segment error detected\nStack segment selector: 0x") + ToString(error, 16) + '\n';
}