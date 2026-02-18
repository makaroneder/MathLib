#ifdef __x86_64__
#include "RTL8139.hpp"
#include "PCI/PCIHeader0.hpp"
#include <String.hpp>
#include <Logger.hpp>

RTL8139::RTL8139(PCIHeader* header) : base(((PCIHeader0*)header)->bar[1]), irq((IRQ)((PCIHeader0*)header)->interruptLine) {
    if (!RegisterIRQDevice(irq, this)) MathLib::Panic("Failed to register IRQ");
    header->memorySpace = true;
    header->busMaster = true;
    header->interruptDisable = false;

    Write<uint8_t>(IOOffset::Configuration1, 0x00);
    Write<uint8_t>(IOOffset::Command, 0x10);
    while (Read<uint8_t>(IOOffset::Command) & 0x10) {}
    Write<uint32_t>(IOOffset::ReceiveBufferStart0, (uintptr_t)rxBuffer);
    Write<uint16_t>(IOOffset::InterruptMask0, 0x0005);
    Write<uint8_t>(IOOffset::Command, 0b1100);
    Write<uint32_t>(IOOffset::ReceiveConfiguration0, 0b10001111);

    for (uint8_t i = 0; i < 6; i++) mac.At(i) = Read<uint8_t>((IOOffset)((uint8_t)IOOffset::ID0 + i));
    LogString("MAC: "_M + mac.ToString() + '\n');
    for (uint8_t i = 0; i < 3; i++)
        Write<uint8_t>(IOOffset::Configuration1, 0x00);
}
RTL8139::~RTL8139(void) {
    if (!RegisterIRQDevice(irq, nullptr)) MathLib::Panic("Failed to unregister IRQ");
}
void RTL8139::OnInterrupt(uintptr_t, Registers*, uintptr_t) {
    MathLib::Panic("Got intterrupt from RTL8139");
}

#endif