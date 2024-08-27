#include "PhysicalFileSystem.hpp"

PhysicalFileSystem::PhysicalFileSystem(ByteDevice& disk) : disk(disk) {}
ByteDevice& PhysicalFileSystem::GetDisk(void) {
    return disk;
}