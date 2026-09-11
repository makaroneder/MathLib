#ifdef __x86_64__
#include "../../../KernelRenderer.hpp"
#include "PS2Mouse.hpp"
#include <Logger.hpp>

PS2Mouse::PS2Mouse(bool second) : PS2Device(second), type(Type::Normal), position(MathLib::CreateVector<ssize_t>(0, 0, 0)), packet(0) {
    for (uint8_t i = 0; i < SizeOfArray(packets); i++) packets[i] = 0;
    // if (!SetSampleRate(200) || !SetSampleRate(100) || !SetSampleRate(80)) MathLib::Panic("Failed to check for Z axis mouse extension");
    // MathLib::Expected<uint16_t> id = GetID();
    // if (!id.HasValue()) MathLib::Panic("Failed to check for Z axis mouse extension");
    // if (id.Get() == 3) {
    //     type = Type::ZAxis;
    //     if (!SetSampleRate(200) || !SetSampleRate(200) || !SetSampleRate(80)) MathLib::Panic("Failed to check for 5 buttons mouse extension");
    //     id = GetID();
    //     if (!id.HasValue()) MathLib::Panic("Failed to check for 5 buttons mouse extension");
    //     if (id.Get() == 4) type = Type::MoreButtons;
    // }
    // else type = Type::Normal;
    // if (!SetSampleRate(200)) MathLib::Panic("Failed to set mouse sample rate");
    if (!RegisterDevice(true)) MathLib::Panic("Failed to register IRQ");
}
PS2Mouse::~PS2Mouse(void) {
    if (!RegisterDevice(false)) MathLib::Panic("Failed to unregister IRQ");
}
void PS2Mouse::OnInterrupt(uintptr_t interrupt, Registers* regs, uintptr_t error) {
    PS2Device::OnInterrupt(interrupt, regs, error);
    const uint8_t tmp = Read().Get("Failed to read mouse packet");
    if (!packet && !(tmp & (1 << (uint8_t)Packet0Bits::Always1))) return;
    packets[packet++] = tmp;
    packet = packet % (type == Type::Normal ? 3 : 4);
    if (packet || (packets[0] & ((1 << (uint8_t)Packet0Bits::OverflowX) | (1 << (uint8_t)Packet0Bits::OverflowY)))) return;

    GetX(position) += (packets[0] & (1 << (uint8_t)Packet0Bits::NegativeX)) ? ((int16_t)packets[1] - 256) : packets[1];
    GetY(position) += (packets[0] & (1 << (uint8_t)Packet0Bits::NegativeY)) ? ((int16_t)packets[2] - 256) : packets[2];

    LogString('['_M + MathLib::ToString(GetX(position)) + ", " + MathLib::ToString(GetY(position)) + "]\n");

    // if (packet1.leftButton && !renderer->AddEvent(MathLib::Event(GetX(position), GetY(position), MathLib::Event::MouseButton::Left, true))) MathLib::Panic("Failed to send event to renderer");
}
bool PS2Mouse::SetSampleRate(uint8_t sampleRate) {
    bool valid = false;
    for (size_t i = 0; i < SizeOfArray(validSampleRates) && !valid; i++)
        if (sampleRate == validSampleRates[i]) valid = true;
    if (!valid) return false;
    MathLib::Expected<uint8_t> tmp = SendCommand(0xf3);
    if (!tmp.HasValue() || tmp.Get() != (uint8_t)Response::Acknowledge) return false;
    tmp = SendCommand(sampleRate);
    return tmp.HasValue() && tmp.Get() == (uint8_t)Response::Acknowledge;
}

#endif