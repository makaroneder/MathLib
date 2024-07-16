#include "Halt.hpp"

__attribute__((noreturn)) void Halt(void) {
    while (true) asm volatile("hlt");
}