#ifdef __x86_64__
#include "PS2.hpp"
#include "../IO.hpp"
#include "PS2Mouse.hpp"
#include "PS2Keyboard.hpp"
#include <String.hpp>
#include <Logger.hpp>

bool Await8042(bool write) {
    size_t timeout = 10000;
    while (timeout-- && ReadPort<uint8_t>(0x64) & (1 << write)) {}
    return timeout;
}
[[nodiscard]] bool Write8042(uint16_t value) {
    if (!Await8042(true)) return false;
    WritePort<uint8_t>(0x64, value);
    const uint8_t tmp = value >> 8;
    if (tmp) {
        if (!Await8042(true)) return false;
        WritePort<uint8_t>(0x60, tmp);
    }
    return true;
}
[[nodiscard]] MathLib::Expected<uint8_t> Read8042(uint8_t cmd) {
    return Write8042(cmd) ? MathLib::Expected<uint8_t>(ReadPort<uint8_t>(0x60)) : MathLib::Expected<uint8_t>();
}
[[nodiscard]] MathLib::String PS2PortToString(bool second) {
    return MathLib::String(second ? "second" : "first") + " PS2 port";
}
[[nodiscard]] bool EnableDevice(bool second) {
    PS2Device device = PS2Device(second);
    const MathLib::Expected<uint8_t> tmp = device.SendCommand(0xf6);
    if (!tmp.HasValue() || tmp.Get() != (uint8_t)PS2Device::Response::Acknowledge) return false;
    const MathLib::Expected<uint16_t> type = device.GetID();
    if (!type.HasValue()) LogString("Found AT keyboard on "_M + PS2PortToString(second) + '\n');
    else switch (type.Get()) {
        case 0x0000:
        case 0x0003:
        case 0x0004: {
            LogString("Found mouse on "_M + PS2PortToString(second) + '\n');
            new PS2Mouse(second);
            break;
        }
        case 0xab83:
        case 0xabc1: {
            LogString("Found MF2 keyboard on "_M + PS2PortToString(second) + '\n');
            new PS2Keyboard(second);
            break;
        }
        case 0xab84: {
            LogString("Found IBM ThinkPad on "_M + PS2PortToString(second) + '\n');
            break;
        }
        case 0xab85: {
            LogString("Found NCD N-97 keyboard on "_M + PS2PortToString(second) + '\n');
            break;
        }
        case 0xab86: {
            LogString("Found 122-key keyboard on "_M + PS2PortToString(second) + '\n');
            break;
        }
        case 0xab90: {
            LogString("Found japanese 'G' keyboard on "_M + PS2PortToString(second) + '\n');
            break;
        }
        case 0xab91: {
            LogString("Found japanese 'P' keyboard on "_M + PS2PortToString(second) + '\n');
            break;
        }
        case 0xab92: {
            LogString("Found japanese 'A' keyboard on "_M + PS2PortToString(second) + '\n');
            break;
        }
        case 0xaca1: {
            LogString("Found NCD sun layout keyboard on "_M + PS2PortToString(second) + '\n');
            break;
        }
        default: LogString("Found unknown device (0x"_M + MathLib::ToString(type.Get(), 16) + ") on " + PS2PortToString(second) + '\n');
    }
    return true;
}
void InitPS2(void) {
    if (!Write8042(0xad) || !Write8042(0xa7) || !Await8042(false)) return;
    while (ReadPort<uint8_t>(0x64) & 1) (void)ReadPort<uint8_t>(0x60);
    MathLib::Expected<uint8_t> tmp = Read8042(0x20);
    if (!tmp.HasValue()) return;
    if (!Write8042(0x60 | (tmp.Get() & ~(1 << 6 | 1 << 4 | 1 << 0)) << 8)) return;
    tmp = Read8042(0xaa);
    if (!tmp.HasValue() || tmp.Get() != 0x55 || !Write8042(0xa8)) return;
    tmp = Read8042(0x20);
    if (!tmp.HasValue()) return;
    const bool doubleDev = !(tmp.Get() & (1 << 5));
    if (doubleDev && (!Write8042(0xa7) || !Write8042(0x60 | (tmp.Get() & ~(1 << 5 | 1 << 1)) << 8))) return;
    tmp = Read8042(0xab);
    if (tmp.HasValue() && !tmp.Get() && Write8042(0xae)) {
        tmp = Read8042(0x20);
        tmp.HasValue() && Write8042(0x60 | (tmp.Get() | 1 << 0) << 8) && EnableDevice(false);
    }
    if (doubleDev) {
        tmp = Read8042(0xa9);
        if (tmp.HasValue() && !tmp.Get() && Write8042(0xa8)) {
            tmp = Read8042(0x20);
            tmp.HasValue() && Write8042(0x60 | (tmp.Get() | 1 << 1) << 8) && EnableDevice(true);
        }
    }
}

#endif