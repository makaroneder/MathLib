#ifdef __x86_64__
#include "ControlRegisters.hpp"

Expected<uintptr_t> GetControlRegister(uint8_t num) {
    uintptr_t val = 0;
    switch (num) {
        case 0: {
            asm volatile("mov %%cr0, %0" : "=r" (val));
            break;
        }
        case 2: {
            asm volatile("mov %%cr2, %0" : "=r" (val));
            break;
        }
        case 3: {
            asm volatile("mov %%cr3, %0" : "=r" (val));
            break;
        }
        case 4: {
            asm volatile("mov %%cr4, %0" : "=r" (val));
            break;
        }
        case 8: {
            asm volatile("mov %%cr8, %0" : "=r" (val));
            break;
        }
        default: return Expected<uintptr_t>();
    }
    return Expected<uintptr_t>(val);
}
bool SetControlRegister(uint8_t num, uintptr_t value) {
    switch (num) {
        case 0: {
            asm volatile("mov %0, %%cr0" : : "r" (value));
            break;
        }
        case 2: {
            asm volatile("mov %0, %%cr2" : : "r" (value));
            break;
        }
        case 3: {
            asm volatile("mov %0, %%cr3" : : "r" (value));
            break;
        }
        case 4: {
            asm volatile("mov %0, %%cr4" : : "r" (value));
            break;
        }
        case 8: {
            asm volatile("mov %0, %%cr8" : : "r" (value));
            break;
        }
        default: return false;
    }
    return true;
}

#endif