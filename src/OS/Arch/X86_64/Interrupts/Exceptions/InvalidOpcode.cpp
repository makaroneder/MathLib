#include "InvalidOpcode.hpp"

InvalidOpcode::InvalidOpcode(void) : Exception(Type::InvalidOpcode) {}
String InvalidOpcode::GetPanicMessage(uintptr_t) const {
    return "Invalid opcode detected\n";
}