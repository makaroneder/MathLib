#ifdef __x86_64__
#include "../../../KernelRenderer.hpp"
#include "PS2Mouse.hpp"

PS2Mouse::PS2Mouse(bool second) : PS2Device(second), position(MathLib::CreateVector<size_t>(0, 0, 0)), packet(0) {
    for (uint8_t i = 0; i < SizeOfArray(packets); i++) packets[i] = 0;
    if (!SetSampleRate(200) || !SetSampleRate(100) || !SetSampleRate(80)) MathLib::Panic("Failed to check for Z axis mouse extension");
    MathLib::Expected<uint16_t> id = GetID();
    if (!id.HasValue()) MathLib::Panic("Failed to check for Z axis mouse extension");
    if (id.Get() == 3) {
        type = Type::ZAxis;
        if (!SetSampleRate(200) || !SetSampleRate(200) || !SetSampleRate(80)) MathLib::Panic("Failed to check for 5 buttons mouse extension");
        id = GetID();
        if (!id.HasValue()) MathLib::Panic("Failed to check for 5 buttons mouse extension");
        if (id.Get() == 4) type = Type::MoreButtons;
    }
    else type = Type::Normal;
    if (!SetSampleRate(200)) MathLib::Panic("Failed to set mouse sample rate");
    RegisterInterruptDevice(GetIRQBase() + 12, this);
}
PS2Mouse::~PS2Mouse(void) {
    RegisterInterruptDevice(GetIRQBase() + 12, nullptr);
}
void PS2Mouse::OnInterrupt(uintptr_t, Registers*, uintptr_t) {
    const uint8_t tmp = Read().Get("Failed to read mouse packet");
    packets[packet++] = tmp;
    packet = packet % (type == Type::Normal ? 3 : 4);
    if (!packet) {
        const PS2MousePacket1 packet1 = *(const PS2MousePacket1*)&packets[0];
        if (!packet1.alwaysOne) MathLib::Panic("Invalid main packet");
        position += MathLib::CreateVector<size_t>(packet1.xSign ? -packets[1] : packets[1], packet1.ySign ? -packets[2] : packets[2], 0);
        if (type == Type::ZAxis) GetZ(position) += (int8_t)packets[3];
        else if (type == Type::MoreButtons) {
            const PS2Mouse5ButtonsPacket packet4 = *(const PS2Mouse5ButtonsPacket*)&packets[3];
            if (packet4.alwaysZero) MathLib::Panic("Invalid 5 buttons packet");
            GetZ(position) += (int8_t)packet4.z;
            if (packet4.button4 && !renderer->AddEvent(MathLib::Event(position, MathLib::Event::MouseButton::Button4, true))) MathLib::Panic("Failed to send event to renderer");
            if (packet4.button5 && !renderer->AddEvent(MathLib::Event(position, MathLib::Event::MouseButton::Button5, true))) MathLib::Panic("Failed to send event to renderer");
        }
        if (packet1.leftButton &&  !renderer->AddEvent(MathLib::Event(position, MathLib::Event::MouseButton::Left, true))) MathLib::Panic("Failed to send event to renderer");
        if (packet1.middleButton &&  !renderer->AddEvent(MathLib::Event(position, MathLib::Event::MouseButton::Middle, true))) MathLib::Panic("Failed to send event to renderer");
        if (packet1.rightButton &&  !renderer->AddEvent(MathLib::Event(position, MathLib::Event::MouseButton::Right, true))) MathLib::Panic("Failed to send event to renderer");
    }
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