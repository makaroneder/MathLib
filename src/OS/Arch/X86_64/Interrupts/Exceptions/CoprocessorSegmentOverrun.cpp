#include "CoprocessorSegmentOverrun.hpp"

CoprocessorSegmentOverrun::CoprocessorSegmentOverrun(void) : Exception(Type::LegacyCoprocessorSegmentOverrun) {}
String CoprocessorSegmentOverrun::GetPanicMessage(uintptr_t) const {
    return "Coprocessor segment overrun detected\n";
}