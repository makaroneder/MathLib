#ifdef __x86_64__
#include "PageFault.hpp"
#include "../../ControlRegisters.hpp"
#include <String.hpp>

MathLib::String PageFaultErrorCode::ToString(const MathLib::String& padding) const {
    MathLib::String ret = padding + "{\n";
    ret += padding + '\t' + (present ? "Page protection violation" : "Non present page accessed") + '\n';
    ret += padding + '\t' + (write ? "Write access" : "Read access") + '\n';
    ret += padding + '\t' + (user ? "Ring 3" : "Ring 0") + '\n';
    if (reservedWrite) ret += padding + "\tInvalid reserved bits\n";
    if (instructionFetch) ret += padding + "\tInstruction fetch error\n";
    if (protectionKey) ret += padding + "\tProtection key violation\n";
    if (shadowStack) ret += padding + "\tShadow stack access\n";
    if (softwareGuardExtensions) ret += padding + "\tSGX violation\n";
    return ret + padding + '}';
}
PageFault::PageFault(void) : Exception(Type::PageFault) {}
MathLib::String PageFault::GetPanicMessage(uintptr_t error) const {
    const MathLib::Expected<uintptr_t> tmp = GetControlRegister(2);
    return tmp.HasValue() ? ("Page fault on address 0x"_M + MathLib::ToString(tmp.Get(), 16) + '\n' + ((PageFaultErrorCode*)&error)->ToString() + '\n') : "Page fault detected\n";
}

#endif