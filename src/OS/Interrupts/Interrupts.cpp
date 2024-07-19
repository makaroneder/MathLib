#include "InterruptDescriptorTableRegister.hpp"
#include "Interrupts.hpp"
#include "PIC8259.hpp"

size_t interruptDisabledCount = 1;
InterruptDescriptorTable idt;
InterruptDescriptorTableRegister idtr = InterruptDescriptorTableRegister(&idt);
PIC8259 pic;
extern "C" uintptr_t isrFunctionTable[];
InterruptDevice* interruptDevices[256];

extern "C" void InterruptHandler(uintptr_t interrupt, Registers* regs, uintptr_t error) {
    if (interruptDevices[interrupt]) interruptDevices[interrupt]->OnInterrupt(interrupt, regs, error);
    interrupt -= pic.GetBase();
    if (interrupt <= 15) pic.SendEndOfInterrupt(interrupt);
}
uint8_t GetIRQBase(void) {
    return pic.GetBase();
}
void SetInterrupts(bool value) {
    if (value) {
        if (!--interruptDisabledCount) asm volatile("sti");
    }
    else if (!interruptDisabledCount++) asm volatile("cli");
}
void RegisterInterruptDevice(uintptr_t interrupt, InterruptDevice* device) {
    interruptDevices[interrupt] = device;
    interrupt -= pic.GetBase();
    if (interrupt <= 15) pic.SetMask(interrupt, !device);
}
void InitInterrupts(uint8_t irqBase, uint8_t codeSegment) {
    pic = PIC8259(irqBase);
    for (uintptr_t i = 0; i < 256; i++)
        idt.descriptors[i] = InterruptDescriptor(isrFunctionTable[i], codeSegment, 1 << 7 | (uint8_t)InterruptDescriptor::GateType::Interrupt);
    asm volatile("lidt %0" : : "m"(idtr));
    SetInterrupts(true);
}