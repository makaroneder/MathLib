#ifdef __x86_64__
#include "PIC.hpp"

PIC::PIC(void) : mask(UINT16_MAX) {}
void PIC::SetIRQMask(uint8_t irq, bool value) {
	if (value) mask |= (1 << irq);
	else mask &= ~(1 << irq);
}
bool PIC::UpdateMask(void) {
    return SetMask(mask);
}

#endif