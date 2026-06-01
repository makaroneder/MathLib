#ifdef __x86_64__
#include "InvalidTSS.hpp"
#include "SelectorErrorCode.hpp"
#include <String.hpp>

InvalidTSS::InvalidTSS(void) : Exception(Type::InvalidTSS) {}
MathLib::String InvalidTSS::GetPanicMessage(uintptr_t error) const {
    return "Invalid TSS used\n"_M + ((SelectorErrorCode*)&error)->ToString() + '\n';
}

#endif