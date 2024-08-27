#include "PIC8259.hpp"
#include "../IO.hpp"
#include <Host.hpp>

PIC8259::PIC8259(void) {}
bool PIC8259::Init(uint8_t base_) {
    if (base_ % 8 != 0) return false;
    base = base_;
    WritePort<uint8_t>((uint16_t)Ports::MasterCommand, (1 << (uint8_t)InitializationControlWord1::Initialize) | (1 << (uint8_t)InitializationControlWord1::UseInitializationControlWord4));
    IOWait();
    WritePort<uint8_t>((uint16_t)Ports::SlaveCommand, (1 << (uint8_t)InitializationControlWord1::Initialize) | (1 << (uint8_t)InitializationControlWord1::UseInitializationControlWord4));
    IOWait();
    WritePort<uint8_t>((uint16_t)Ports::MasterData, base);
    IOWait();
    WritePort<uint8_t>((uint16_t)Ports::SlaveData, base + 8);
    IOWait();
    WritePort<uint8_t>((uint16_t)Ports::MasterData, 4);
    IOWait();
    WritePort<uint8_t>((uint16_t)Ports::SlaveData, 2);
    IOWait();
    WritePort<uint8_t>((uint16_t)Ports::MasterData, 1 << (uint8_t)InitializationControlWord4::Mode8086);
    IOWait();
    WritePort<uint8_t>((uint16_t)Ports::SlaveData, 1 << (uint8_t)InitializationControlWord4::Mode8086);
    IOWait();
    return UpdateMask();
}
uint8_t PIC8259::GetBase(void) const {
    return base;
}
bool PIC8259::SendEndOfInterrupt(uint8_t irq) {
    if (irq > 15) return false;
    if (irq >= 8) WritePort<uint8_t>((uint16_t)Ports::SlaveCommand, (uint8_t)Command::EndOfInterrupt);
    WritePort<uint8_t>((uint16_t)Ports::MasterCommand, (uint8_t)Command::EndOfInterrupt);
    return true;
}
bool PIC8259::SetMask(uint16_t mask) {
    WritePort<uint8_t>((uint16_t)Ports::MasterData, mask);
    WritePort<uint8_t>((uint16_t)Ports::SlaveData, mask >> 8);
    return true;
}