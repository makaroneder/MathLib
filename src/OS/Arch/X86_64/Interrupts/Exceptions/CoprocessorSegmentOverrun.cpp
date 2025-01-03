#ifdef __x86_64__
#include "CoprocessorSegmentOverrun.hpp"

CoprocessorSegmentOverrun::CoprocessorSegmentOverrun(void) : Exception(Type::LegacyCoprocessorSegmentOverrun) {}
MathLib::String CoprocessorSegmentOverrun::GetPanicMessage(uintptr_t) const {
    return "Coprocessor segment overrun detected\n";
}

#endif