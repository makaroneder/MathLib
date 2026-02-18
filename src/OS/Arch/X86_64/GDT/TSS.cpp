#ifdef __x86_64__
#include "TSS.hpp"
#include "GDT.hpp"

TSS tss;
void InitTSS(void) {
    gdt.tss.SetBase((uintptr_t)&tss);
    asm volatile("ltr %%ax" :: "a"(0x18));
}

#endif