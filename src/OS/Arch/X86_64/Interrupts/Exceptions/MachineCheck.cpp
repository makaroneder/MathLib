#ifdef __x86_64__
#include "MachineCheck.hpp"
#include "../../ControlRegisters.hpp"

MachineCheck::MachineCheck(void) : Exception(Type::MachineCheck) {}
bool MachineCheck::Enable(void) {
    const MathLib::Expected<uintptr_t> tmp = GetControlRegister(4);
    if (!tmp.HasValue()) return false;
    return SetControlRegister(4, tmp.Get() | 1 << (uint8_t)ControlRegister4::MachineCheckException);
}
MathLib::String MachineCheck::GetPanicMessage(uintptr_t) const {
    return "CPU internal error detected\n";
}

#endif