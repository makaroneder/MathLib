#include "PIC8259.hpp"
#include "../IO.hpp"

PIC8259::PIC8259(void) {}
PIC8259::PIC8259(uint8_t base) : base(base) {
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
	WritePort<uint8_t>((uint16_t)Ports::MasterData, UINT8_MAX);
	WritePort<uint8_t>((uint16_t)Ports::SlaveData, UINT8_MAX);
}
bool PIC8259::SendEndOfInterrupt(uint8_t irq) {
	if (irq > 15) return false;
	if (irq >= 8) WritePort<uint8_t>((uint16_t)Ports::SlaveCommand, (uint8_t)Command::EndOfInterrupt);
	WritePort<uint8_t>((uint16_t)Ports::MasterCommand, (uint8_t)Command::EndOfInterrupt);
	return true;
}
bool PIC8259::SetMask(uint8_t irq, bool value) {
	if (irq > 15) return false;
	Ports port;
	if (irq < 8) port = Ports::MasterData;
	else {
		port = Ports::SlaveData;
		irq -= 8;
	}
	uint8_t mask = ReadPort<uint8_t>((uint16_t)port);
	if (value) mask |= (1 << irq);
	else mask &= ~(1 << irq);
	WritePort<uint8_t>((uint16_t)port, value);
	return true;
}
uint8_t PIC8259::GetBase(void) const {
	return base;
}