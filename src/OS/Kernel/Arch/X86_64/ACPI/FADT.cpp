#ifdef __x86_64__
#include "FADT.hpp"

FACS* FADT::GetFACS(void) {
    return (FACS*)(uintptr_t)facs;
}
MathLib::DSDT* FADT::GetDSDT(void) {
    return (MathLib::DSDT*)(uintptr_t)dsdt;
}
FACS* FADT2::GetFACS(void) {
    return facs2 ? (FACS*)facs2 : FADT::GetFACS();
}
MathLib::DSDT* FADT2::GetDSDT(void) {
    return dsdt2 ? (MathLib::DSDT*)dsdt2 : FADT::GetDSDT();
}

#endif