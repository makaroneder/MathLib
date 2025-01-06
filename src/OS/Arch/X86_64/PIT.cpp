#ifdef __x86_64__
#include "PIT.hpp"
#include "IO.hpp"
#include <Host.hpp>

PIT::PIT(void) {
    SetInterrupts(false);
    WritePort<uint8_t>((uint16_t)Ports::CommandRegister, ((uint8_t)OperatingMode::RateGenerator << 1) | ((uint8_t)AccessMode::LowAndHighByte << 4) | ((uint8_t)Channel::Channel0 << 6));
    if (!SetReloadValue(UINT16_MAX + 1)) MathLib::Panic("Failed to set PIT reaload value");
    RegisterInterruptDevice(GetIRQBase(), this);
    SetInterrupts(true);
}
PIT::~PIT(void) {
    RegisterInterruptDevice(GetIRQBase(), nullptr);
}
MathLib::num_t PIT::GetFrequency(void) const {
    return baseFrequency / GetReloadValue();
}
bool PIT::SetFrequency(MathLib::num_t frequency) {
    return SetReloadValue(baseFrequency / frequency);
}
size_t PIT::GetReloadValue(void) const {
    SetInterrupts(false);
    const uint16_t ret = ReadPort<uint8_t>((uint16_t)Ports::Channel0) | (ReadPort<uint8_t>((uint16_t)Ports::Channel0) << 8);
    SetInterrupts(true);
    return ret ? ret : UINT16_MAX + 1;
}
bool PIT::SetReloadValue(size_t value) {
    if (value > UINT16_MAX + 1 || !value) return false;
    if (value == UINT16_MAX + 1) value = 0;
    SetInterrupts(false);
    WritePort<uint8_t>((uint16_t)Ports::Channel0, value);
	WritePort<uint8_t>((uint16_t)Ports::Channel0, value >> 8);
    SetInterrupts(true);
    return true;
}

#endif