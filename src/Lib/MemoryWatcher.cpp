#include "MemoryWatcher.hpp"

MemoryWatcher* memoryWatcher;
MemoryWatcher::MemoryWatcher(void) {
    memoryWatcher = this;
}
MemoryWatcher::~MemoryWatcher(void) {}