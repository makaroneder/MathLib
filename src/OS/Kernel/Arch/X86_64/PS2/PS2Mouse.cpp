#ifdef __x86_64__
#include "../../../KernelRenderer.hpp"
#include "PS2Mouse.hpp"

PS2Mouse::PS2Mouse(bool second) : PS2Device(second), type(Type::Normal), position(MathLib::CreateVector<ssize_t>(0, 0, 0)), packet(0), leftButton(false), rightButton(false), middleButton(false), button4(false), button5(false) {
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
    if (!SetSampleRate(60)) MathLib::Panic("Failed to set mouse sample rate");
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
    packet = packet % (3 + (type != Type::Normal));
    if (packet || (packets[0] & ((1 << (uint8_t)Packet0Bits::OverflowX) | (1 << (uint8_t)Packet0Bits::OverflowY)))) return;
    GetX(position) += ((packets[0] & (1 << (uint8_t)Packet0Bits::NegativeX)) ? ((int16_t)packets[1] - 256) : packets[1]);
    if (GetX(position) <= 0) GetX(position) = 0;
    if (renderer && renderer->GetWidth() && (size_t)GetX(position) >= renderer->GetWidth()) GetX(position) = renderer->GetWidth() - 1;
    GetY(position) -= ((packets[0] & (1 << (uint8_t)Packet0Bits::NegativeY)) ? ((int16_t)packets[2] - 256) : packets[2]);
    if (GetY(position) <= 0) GetY(position) = 0;
    if (renderer && renderer->GetHeight() && (size_t)GetY(position) >= renderer->GetHeight()) GetY(position) = renderer->GetHeight() - 1;
    if (!renderer) return;
    bool none = true;
    if (leftButton != !!(packets[0] & (1 << (uint8_t)Packet0Bits::LeftButton))) {
        none = false;
        leftButton = !leftButton;
        if (!renderer->AddEvent(MathLib::Event(GetX(position), GetY(position), MathLib::Event::MouseButton::Left, leftButton))) MathLib::Panic("Failed to send event to renderer");
    }
    if (rightButton != !!(packets[0] & (1 << (uint8_t)Packet0Bits::RightButton))) {
        none = false;
        rightButton = !rightButton;
        if (!renderer->AddEvent(MathLib::Event(GetX(position), GetY(position), MathLib::Event::MouseButton::Right, rightButton))) MathLib::Panic("Failed to send event to renderer");
    }
    if (middleButton != !!(packets[0] & (1 << (uint8_t)Packet0Bits::MiddleButton))) {
        none = false;
        middleButton = !middleButton;
        if (!renderer->AddEvent(MathLib::Event(GetX(position), GetY(position), MathLib::Event::MouseButton::Middle, middleButton))) MathLib::Panic("Failed to send event to renderer");
    }
    if (type == Type::MoreButtons) {
        if (button4 != !!(packets[3] & (1 << (uint8_t)Packet3Bits::Button4))) {
            none = false;
            button4 = !button4;
            if (!renderer->AddEvent(MathLib::Event(GetX(position), GetY(position), MathLib::Event::MouseButton::Button4, button4))) MathLib::Panic("Failed to send event to renderer");
        }
        if (button5 != !!(packets[3] & (1 << (uint8_t)Packet3Bits::Button5))) {
            none = false;
            button5 = !button5;
            if (!renderer->AddEvent(MathLib::Event(GetX(position), GetY(position), MathLib::Event::MouseButton::Button5, button5))) MathLib::Panic("Failed to send event to renderer");
        }
    }
    if (none && !renderer->AddEvent(MathLib::Event((size_t)GetX(position), (size_t)GetY(position)))) MathLib::Panic("Failed to send event to renderer");
}
bool PS2Mouse::SetSampleRate(uint8_t sampleRate) {
    uint8_t best = UINT8_MAX;
    uint16_t delta = UINT16_MAX;
    for (size_t i = 0; i < SizeOfArray(validSampleRates); i++) {
        const uint16_t tmp = validSampleRates[i] <= sampleRate ? sampleRate - validSampleRates[i] : validSampleRates[i] - sampleRate;
        if (tmp > delta) continue;
        best = validSampleRates[i];
        delta = tmp;
    }
    return best != UINT8_MAX && SendCommand(0xf3).GetOr((uint8_t)Response::Resend) == (uint8_t)Response::Acknowledge && SendCommand(best).GetOr((uint8_t)Response::Resend) == (uint8_t)Response::Acknowledge;
}

#endif