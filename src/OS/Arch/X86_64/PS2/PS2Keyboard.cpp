#include "../../../KernelRenderer.hpp"
#include "PS2Keyboard.hpp"

PS2Keyboard::PS2Keyboard(bool second) : PS2Device(second) {
    Expected<uint8_t> tmp = SendCommand(0xf0);
    if (!tmp.HasValue() || tmp.Get() != (uint8_t)Response::Acknowledge) Panic("Failed to set scan code set");
    tmp = SendCommand(0b01);
    if (!tmp.HasValue() || tmp.Get() != (uint8_t)Response::Acknowledge) Panic("Failed to set scan code set");
    RegisterInterruptDevice(GetIRQBase() + 1, this);
}
PS2Keyboard::~PS2Keyboard(void) {
    RegisterInterruptDevice(GetIRQBase() + 1, nullptr);
}
char PS2Keyboard::ToKey(const uint8_t& code) {
    // TODO: Handle upper case letters
    if (code < SizeOfArray(scanCodeSet1)) return scanCodeSet1[code];
    return '\0';
}
void PS2Keyboard::OnInterrupt(uintptr_t, Registers*, uintptr_t) {
    uint8_t code = Read().Get("Failed to read keyboard scancode");
    bool pressed = true;
    if (code > 0x80) {
        pressed = false;
        code -= 0x80;
    }
    const char key = ToKey(code);
    if (key && renderer && !renderer->AddEvent(Event(key, pressed))) Panic("Failed to add keyboard event");
}