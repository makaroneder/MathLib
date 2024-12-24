#include "PhysicalFileSystem.hpp"

namespace MathLib {
    PhysicalFileSystem::PhysicalFileSystem(ByteDevice& disk) : disk(disk) {
        EmptyBenchmark
    }
    ByteDevice& PhysicalFileSystem::GetDisk(void) {
        StartBenchmark
        ReturnFromBenchmark(disk);
    }
}