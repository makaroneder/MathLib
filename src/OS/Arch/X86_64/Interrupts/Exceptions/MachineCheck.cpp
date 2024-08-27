#include "MachineCheck.hpp"
#include "../../ControlRegisters.hpp"

MachineCheck::MachineCheck(void) : Exception(Type::MachineCheck) {}
void MachineCheck::Enable(void) {
    SetControlRegister(4, GetControlRegister(4) | 1 << (uint8_t)ControlRegister4::MachineCheckException);
}
String MachineCheck::GetPanicMessage(uintptr_t) const {
    return "CPU internal error detected\n";
}