#ifdef __x86_64__
#include "GDT.hpp"

TSS* GDT::GetTSS(void) {
    return (TSS*)tss.GetBase();
}

#endif