#ifndef Emulator_Emulator_H
#define Emulator_Emulator_H
#include "../Printable.hpp"

struct Emulator : Printable {
    Emulator(const Array<uint8_t>& mem);
    virtual bool Step(void) = 0;
    size_t GetSize(void) const;

    protected:
    Array<uint8_t> memory;
};

#endif