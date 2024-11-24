#include "Allocatable.hpp"
#define WatchMemory(event, ...) if (memoryWatcher) memoryWatcher->event(__VA_ARGS__)

namespace MathLib {
    Allocatable::~Allocatable(void) {}
    void* Allocatable::operator new(size_t size) {
        void* ret = ::operator new(size);
        WatchMemory(OnAlloc, ret, size);
        return ret;
    }
    void* Allocatable::operator new[](size_t size) {
        void* ret = ::operator new[](size);
        WatchMemory(OnAlloc, ret, size);
        return ret;
    }
    void Allocatable::operator delete(void* ptr) {
        WatchMemory(OnDealloc, ptr);
        ::operator delete(ptr);
    }
    void Allocatable::operator delete[](void* ptr) {
        WatchMemory(OnDealloc, ptr);
        ::operator delete[](ptr);
    }
}