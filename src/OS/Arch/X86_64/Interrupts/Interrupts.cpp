#include "Exceptions/HypervisorInjectionException.hpp"
#include "Exceptions/ControlProtectionException.hpp"
#include "Exceptions/CoprocessorSegmentOverrun.hpp"
#include "Exceptions/VMMCommunicationException.hpp"
#include "Exceptions/VirtualizationException.hpp"
#include "Exceptions/GeneralProtectionFault.hpp"
#include "InterruptDescriptorTableRegister.hpp"
#include "Exceptions/NonMaskableInterrupt.hpp"
#include "Exceptions/DeviceNotAvailable.hpp"
#include "Exceptions/SIMDFloatException.hpp"
#include "Exceptions/StackSegmentFault.hpp"
#include "Exceptions/SegmentNotPresent.hpp"
#include "Exceptions/X87FloatException.hpp"
#include "Exceptions/SecurityException.hpp"
#include "Exceptions/BoundRangeError.hpp"
#include "Exceptions/DebugException.hpp"
#include "Exceptions/AlignmentCheck.hpp"
#include "Exceptions/DivisionError.hpp"
#include "Exceptions/InvalidOpcode.hpp"
#include "Exceptions/MachineCheck.hpp"
#include "Exceptions/DoubleFault.hpp"
#include "Exceptions/InvalidTSS.hpp"
#include "Exceptions/Breakpoint.hpp"
#include "Exceptions/PageFault.hpp"
#include "Exceptions/Overflow.hpp"
#include "SystemCall.hpp"
#include "Interrupts.hpp"
#include "PIC8259.hpp"
#include "Cascade.hpp"
#include "../CPU.hpp"
#include <Host.hpp>

bool initialized = false;
size_t interruptDisabledCount = 1;
InterruptDescriptorTable idt;
InterruptDescriptorTableRegister idtr = InterruptDescriptorTableRegister(&idt);
PIC8259 pic;
extern "C" uintptr_t isrFunctionTable[];
InterruptDevice* interruptDevices[256];
DivisionError divisionError;
DebugException debugException;
NonMaskableInterrupt nonMaskableInterrupt;
Breakpoint breakpoint;
Overflow overflow;
BoundRangeError boundRangeError;
InvalidOpcode invalidOpcode;
DeviceNotAvailable deviceNotAvailable;
DoubleFault doubleFault;
CoprocessorSegmentOverrun coprocessorSegmentOverrun;
InvalidTSS invalidTSS;
SegmentNotPresent segmentNotPresent;
StackSegmentFault stackSegmentFault;
GeneralProtectionFault generalProtectionFault;
PageFault pageFault;
X87FloatException x87FloatException;
AlignmentCheck alignmentCheck;
MachineCheck machineCheck;
SIMDFloatException simdFloatException;
VirtualizationException virtualizationException;
ControlProtectionException controlProtectionException;
HypervisorInjectionException hypervisorInjectionException;
VMMCommunicationException vmmCommunicationException;
SecurityException securityException;
Cascade cascade;
SystemCall systemCall;

extern "C" void InterruptHandler(uintptr_t interrupt, Registers* regs, uintptr_t error) {
    if (interruptDevices[interrupt]) interruptDevices[interrupt]->OnInterrupt(interrupt, regs, error);
    interrupt -= pic.GetBase();
    if (interrupt <= 15) pic.SendEndOfInterrupt(interrupt);
}
uint8_t GetIRQBase(void) {
    return pic.GetBase();
}
void SetInterrupts(bool value) {
    Flags flags = GetFlags();
    if (value) {
        if (!--interruptDisabledCount) flags.interruptEnable = true;
    }
    else if (!interruptDisabledCount++) flags.interruptEnable = false;
    SetFlags(flags.value);
}
void RegisterInterruptDevice(uintptr_t interrupt, InterruptDevice* device) {
    interruptDevices[interrupt] = device;
    interrupt -= pic.GetBase();
    if (interrupt <= 15) {
        pic.SetIRQMask(interrupt, !device);
        if (initialized && !pic.UpdateMask()) Panic("Failed to update PIC interrupt mask");
    }
}
bool InitInterrupts(uint8_t irqBase, uint8_t codeSegment) {
    if (initialized || !pic.Init(irqBase)) return false;
    for (uintptr_t i = 0; i < 256; i++)
        idt.descriptors[i] = InterruptDescriptor(isrFunctionTable[i], codeSegment, 1 << 7 | (uint8_t)InterruptDescriptor::GateType::Interrupt);
    asm volatile("lidt %0" : : "m"(idtr));
    if (!AlignmentCheck::Enable() || !MachineCheck::Enable()) return false;
    SetInterrupts(true);
    initialized = true;
    return true;
}