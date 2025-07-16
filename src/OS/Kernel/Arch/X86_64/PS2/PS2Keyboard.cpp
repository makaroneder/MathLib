#ifdef __x86_64__
#include "../../../KernelRenderer.hpp"
#include "PS2Keyboard.hpp"

PS2Keyboard::PS2Keyboard(bool second) : PS2Device(second), leftShift(false), rightShift(false), capslock(false) {
    MathLib::Expected<uint8_t> tmp = SendCommand(0xf0);
    if (!tmp.HasValue() || tmp.Get() != (uint8_t)Response::Acknowledge) MathLib::Panic("Failed to set scan code set");
    tmp = SendCommand(0b01);
    if (!tmp.HasValue() || tmp.Get() != (uint8_t)Response::Acknowledge) MathLib::Panic("Failed to set scan code set");
    if (!RegisterDevice(true)) MathLib::Panic("Failed to register IRQ");
}
PS2Keyboard::~PS2Keyboard(void) {
    if (!RegisterDevice(false)) MathLib::Panic("Failed to unregister IRQ");
}
void PS2Keyboard::OnInterrupt(uintptr_t interrupt, Registers* regs, uintptr_t error) {
    PS2Device::OnInterrupt(interrupt, regs, error);
    uint8_t code = Read().Get("Failed to read keyboard scancode");
    bool pressed = true;
    if (code > 0x80) {
        pressed = false;
        code -= 0x80;
    }
    char key = '\0';
    if (code == (uint8_t)SpecialCodes::LeftShift) leftShift = pressed;
    else if (code == (uint8_t)SpecialCodes::RightShift) rightShift = pressed;
    else if (code == (uint8_t)SpecialCodes::Capslock && pressed) capslock = !capslock;
    else if (code < SizeOfArray(scanCodeSet1)) key = scanCodeSet1[code];
    if (key && (leftShift || rightShift || capslock)) key = MathLib::ToUpper(key);
    if (leftShift || rightShift) {
        switch (key) {
            case '1': {
                key = '!';
                break;
            }
            case '2': {
                key = '@';
                break;
            }
            case '3': {
                key = '#';
                break;
            }
            case '4': {
                key = '$';
                break;
            }
            case '5': {
                key = '%';
                break;
            }
            case '6': {
                key = '^';
                break;
            }
            case '7': {
                key = '&';
                break;
            }
            case '8': {
                key = '*';
                break;
            }
            case '9': {
                key = '(';
                break;
            }
            case '0': {
                key = ')';
                break;
            }
            case '`': {
                key = '~';
                break;
            }
            case '-': {
                key = '_';
                break;
            }
            case '+': {
                key = '=';
                break;
            }
            case '[': {
                key = '{';
                break;
            }
            case ']': {
                key = '}';
                break;
            }
            case '\\': {
                key = '|';
                break;
            }
            case ';': {
                key = ':';
                break;
            }
            case '\'': {
                key = '"';
                break;
            }
            case ',': {
                key = '<';
                break;
            }
            case '.': {
                key = '>';
                break;
            }
            case '/': {
                key = '?';
                break;
            }
            default: break;
        }
    }
    if (key && renderer && !renderer->AddEvent(MathLib::Event(key, pressed))) MathLib::Panic("Failed to add keyboard event");
}

#endif