#ifndef Emulator_Emulator_H
#define Emulator_Emulator_H
#include "../Printable.hpp"

struct Emulator : Printable {
    Emulator(Array<uint8_t> mem);
    virtual bool Step(void) = 0;

    Array<uint8_t> memory;
};

#endif