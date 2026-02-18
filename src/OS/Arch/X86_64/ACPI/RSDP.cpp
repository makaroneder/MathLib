#ifdef __x86_64__
#include "RSDP.hpp"
#include <stddef.h>

bool RSDP::IsValid(void) const {
    for (size_t i = 0; expectedSignature[i]; i++)
        if (signature[i] != expectedSignature[i]) return false;
    uint8_t sum = 0;
    const uint8_t* self8 = (const uint8_t*)this;
    for (size_t i = 0; i < sizeof(RSDP); i++) sum += self8[i];
    return !sum;
}
bool RSDP2::IsValid(void) const {
    if (!RSDP::IsValid()) return false;
    uint8_t sum = 0;
    const uint8_t* self8 = (const uint8_t*)this;
    for (size_t i = 0; i < length; i++) sum += self8[i];
    return !sum;
}
RSDP* FindRSDP(void) {
    for (RSDP* rsdp = (RSDP*)0xe0000; (uintptr_t)rsdp < 0xfffff; rsdp = (RSDP*)((uintptr_t)rsdp + 16))
        if (rsdp->IsValid()) return rsdp;
    return nullptr;
}

#endif