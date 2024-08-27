#include "PS2Device.hpp"
#include "../IO.hpp"
#include "PS2.hpp"

PS2Device::PS2Device(bool second) : second(second) {}
Expected<uint8_t> PS2Device::Read(void) const {
    size_t timeout = 10000;
    while (timeout-- && !(ReadPort<uint8_t>(0x64) & 1)) {}
    if (!timeout) return Expected<uint8_t>();
    return Expected<uint8_t>(ReadPort<uint8_t>(0x60));
}
bool PS2Device::Write(uint8_t value) {
    return SendPS2Data(value, second);
}
Expected<uint8_t> PS2Device::SendCommand(uint8_t command) {
    if (!Write(command)) return Expected<uint8_t>();
    else return Read();
}
Expected<uint16_t> PS2Device::GetID(void) {
    Expected<uint8_t> tmp = SendCommand(0xf5);
    if (!tmp.HasValue() || tmp.Get() != (uint8_t)Response::Acknowledge) return Expected<uint16_t>();
    tmp = SendCommand(0xf2);
    if (!tmp.HasValue() || tmp.Get() != (uint8_t)Response::Acknowledge) return Expected<uint16_t>();
    tmp = Read();
    if (!tmp.HasValue()) return Expected<uint16_t>();
    uint16_t type = tmp.Get();
    if (type == 0xab || type == 0xac) {
        tmp = Read();
        if (!tmp.HasValue()) return Expected<uint16_t>();
        type = tmp.Get() | type << 8;
    }
    tmp = SendCommand(0xf4);
    if (!tmp.HasValue() || tmp.Get() != (uint8_t)Response::Acknowledge) return Expected<uint16_t>();
    else return Expected<uint16_t>(type);
}