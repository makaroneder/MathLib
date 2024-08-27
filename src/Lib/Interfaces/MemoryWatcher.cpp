#include "MemoryWatcher.hpp"

MemoryWatcher* memoryWatcher = nullptr;
MemoryWatcher::MemoryWatcher(void) {
    memoryWatcher = this;
}
MemoryWatcher::~MemoryWatcher(void) {}