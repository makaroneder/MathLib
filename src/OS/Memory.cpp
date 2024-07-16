#include "Memory.hpp"
#include <stdint.h>

uint8_t memory[2000 * 4096];
size_t pos = 0;
void* operator new(size_t size) {
    void* ret = &memory[pos];
    pos += size;
    return ret;
}
void* operator new[](size_t size) {
    void* ret = &memory[pos];
    pos += size;
    return ret;
}
void operator delete(void* ptr) {
    // TODO:
    (void)ptr;
}
void operator delete[](void* ptr) {
    // TODO:
    (void)ptr;
}
void operator delete(void* ptr, size_t size) {
    // TODO:
    (void)ptr;
    (void)size;
}
void operator delete[](void* ptr, size_t size) {
    // TODO:
    (void)ptr;
    (void)size;
}