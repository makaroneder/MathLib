#include "AlignmentCheck.hpp"
#include "../../ControlRegisters.hpp"
#include "../../CPU.hpp"
#include <Emulator/Flags.hpp>

AlignmentCheck::AlignmentCheck(void) : Exception(Type::AlignmentCheck) {}
void AlignmentCheck::Enable(void) {
    SetControlRegister(0, GetControlRegister(0) | 1 << (uint8_t)ControlRegister0::AlignmentMask);
    Flags flags = GetFlags();
    flags.alignmentCheck = true;
    SetFlags(flags.value);
}
String AlignmentCheck::GetPanicMessage(uintptr_t) const {
    return "Unaligned memory data reference detected\n";
}