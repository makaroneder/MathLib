#include "InvalidTSS.hpp"
#include <String.hpp>

InvalidTSS::InvalidTSS(void) : Exception(Type::InvalidTSS) {}
String InvalidTSS::GetPanicMessage(uintptr_t error) const {
    return String("Invalid TSS used\nTSS selector: 0x") + ToString(error, 16) + '\n';
}