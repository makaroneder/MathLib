#ifdef __x86_64__
#include "InvalidTSS.hpp"
#include "SelectorErrorCode.hpp"
#include <String.hpp>

InvalidTSS::InvalidTSS(void) : Exception(Type::InvalidTSS) {}
MathLib::String InvalidTSS::GetPanicMessage(uintptr_t error) const {
    return MathLib::String("Invalid TSS used\n") + ((SelectorErrorCode*)&error)->ToString() + '\n';
}

#endif