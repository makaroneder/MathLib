#include "PhysicalFileSystem.hpp"

namespace MathLib {
    PhysicalFileSystem::PhysicalFileSystem(ByteDevice& disk) : disk(disk) {}
    ByteDevice& PhysicalFileSystem::GetDisk(void) {
        return disk;
    }
}