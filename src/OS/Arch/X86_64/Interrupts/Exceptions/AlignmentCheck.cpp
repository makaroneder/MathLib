#include "AlignmentCheck.hpp"
#include "../../ControlRegisters.hpp"
#include "../../CPU.hpp"
#include <Emulator/X86/Flags.hpp>

AlignmentCheck::AlignmentCheck(void) : Exception(Type::AlignmentCheck) {}
bool AlignmentCheck::Enable(void) {
    const Expected<uintptr_t> tmp = GetControlRegister(0);
    if (!tmp.HasValue()) return false;
    if (!SetControlRegister(0, tmp.Get() | 1 << (uint8_t)ControlRegister0::AlignmentMask)) return false;
    Flags flags = GetFlags();
    flags.alignmentCheck = true;
    SetFlags(flags.value);
    return true;
}
String AlignmentCheck::GetPanicMessage(uintptr_t) const {
    return "Unaligned memory data reference detected\n";
}