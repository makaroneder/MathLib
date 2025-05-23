#ifdef __x86_64__
#ifndef PS2Keyboard_H
#define PS2Keyboard_H
#include "PS2Device.hpp"

struct PS2Keyboard : PS2Device {
    PS2Keyboard(bool second);
    virtual ~PS2Keyboard(void) override;
    virtual void OnInterrupt(uintptr_t interrupt, Registers* regs, uintptr_t error) override;

    private:
    bool leftShift;
    bool rightShift;
    bool capslock;

    enum class SpecialCodes : uint8_t {
        LeftShift = 0x2a,
        RightShift = 0x36,
        Capslock = 0x3a,
    };
    static constexpr const char scanCodeSet1[] = {
        '\0', '\0', '1', '2',
        '3', '4', '5', '6',
        '7', '8', '9', '0',
        '-', '=', '\b', '\t',
        'q', 'w', 'e', 'r',
        't', 'y', 'u', 'i',
        'o', 'p', '[', ']',
        '\n', '\0', 'a', 's',
        'd', 'f', 'g', 'h',
        'j', 'k', 'l', ';',
        '\'', '`', '\0', '\\',
        'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',',
        '.', '/', '\0', '*',
        '\0', ' ',
    };
};

#endif
#endif