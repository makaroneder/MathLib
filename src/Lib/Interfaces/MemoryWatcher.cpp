#include "MemoryWatcher.hpp"

namespace MathLib {
    MemoryWatcher* memoryWatcher = nullptr;
    MemoryWatcher::MemoryWatcher(void) {
        memoryWatcher = this;
    }
    MemoryWatcher::~MemoryWatcher(void) {}
}