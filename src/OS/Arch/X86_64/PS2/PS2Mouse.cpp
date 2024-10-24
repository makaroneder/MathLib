#ifdef __x86_64__
#include "PS2Mouse.hpp"
#include <Logger.hpp>

PS2Mouse::PS2Mouse(bool second) : PS2Device(second), packet(0) {
    for (uint8_t i = 0; i < SizeOfArray(packets); i++) packets[i] = 0;
    if (!SetSampleRate(200) || !SetSampleRate(100) || !SetSampleRate(80)) Panic("Failed to check for Z axis mouse extension");
    Expected<uint16_t> id = GetID();
    if (!id.HasValue()) Panic("Failed to check for Z axis mouse extension");
    if (id.Get() == 3) {
        type = Type::ZAxis;
        if (!SetSampleRate(200) || !SetSampleRate(200) || !SetSampleRate(80)) Panic("Failed to check for 5 buttons mouse extension");
        id = GetID();
        if (!id.HasValue()) Panic("Failed to check for 5 buttons mouse extension");
        if (id.Get() == 4) type = Type::MoreButtons;
    }
    else type = Type::Normal;
    if (!SetSampleRate(200)) Panic("Failed to set mouse sample rate");
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
        // TODO: Use data instead of logging it
        const PS2MousePacket1 packet1 = *(const PS2MousePacket1*)&packets[0];
        if (!packet1.alwaysOne) Panic("Invalid main packet");
        LogString("Mouse packets:\n");
        LogString(String("\tLeft button: ") + (packet1.leftButton ? "pressed" : "released") + '\n');
        LogString(String("\tRight button: ") + (packet1.rightButton ? "pressed" : "released") + '\n');
        LogString(String("\tMiddle button: ") + (packet1.middleButton ? "pressed" : "released") + '\n');
        LogString(String("\tX sign bit: ") + (packet1.xSign + '0') + '\n');
        LogString(String("\tY sign bit: ") + (packet1.ySign + '0') + '\n');
        LogString(String("\tX overflow bit: ") + (packet1.xOverflow + '0') + '\n');
        LogString(String("\tY overflow bit: ") + (packet1.yOverflow + '0') + '\n');
        LogString(String("\tX: ") + ToString(packets[1]) + '\n');
        LogString(String("\tY: ") + ToString(packets[2]) + '\n');
        if (type == Type::ZAxis) LogString(String("\tZ: ") + ToString(packets[3]) + '\n');
        else if (type == Type::MoreButtons) {
            const PS2Mouse5ButtonsPacket packet4 = *(const PS2Mouse5ButtonsPacket*)&packets[3];
            if (packet4.alwaysZero) Panic("Invalid 5 buttons packet");
            LogString(String("\tZ: ") + ToString(packet4.z) + '\n');
            LogString(String("\t4 button: ") + (packet4.button4 ? "pressed" : "released") + '\n');
            LogString(String("\t5 button: ") + (packet4.button5 ? "pressed" : "released") + '\n');
        }
    }
}
bool PS2Mouse::SetSampleRate(uint8_t sampleRate) {
    bool valid = false;
    for (size_t i = 0; i < SizeOfArray(validSampleRates) && !valid; i++)
        if (sampleRate == validSampleRates[i]) valid = true;
    if (!valid) return false;
    Expected<uint8_t> tmp = SendCommand(0xf3);
    if (!tmp.HasValue() || tmp.Get() != (uint8_t)Response::Acknowledge) return false;
    tmp = SendCommand(sampleRate);
    return tmp.HasValue() && tmp.Get() == (uint8_t)Response::Acknowledge;
}

#endif