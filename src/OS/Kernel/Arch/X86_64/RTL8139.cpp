#ifdef __x86_64__
#include "RTL8139.hpp"
#include "PCI/PCIHeader0.hpp"
#include <String.hpp>
#include <Logger.hpp>

RTL8139::RTL8139(PCIHeader* header) : base(((PCIHeader0*)header)->bar[1]) {
    Write<uint8_t>(IOOffset::Configuration1, 0x00);
    Write<uint8_t>(IOOffset::Command, 0x10);
    while (Read<uint8_t>(IOOffset::Command) & 0x10) {}

    // TODO: Init receive buffer
    // TODO: Set IMR
    // TODO: Set ISR
    // TODO: Configure receive buffer
    // TODO: Enable receive and trasmitter
    // TODO: Interrupt handler

    LogString("MAC: 0x");
    for (uint8_t i = 0; i < 6; i++) LogString(MathLib::ToString(Read<uint8_t>((IOOffset)((uint8_t)IOOffset::ID0 + i)), 16));
    LogChar('\n');
}

#endif