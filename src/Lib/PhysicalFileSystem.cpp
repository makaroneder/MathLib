#include "PhysicalFileSystem.hpp"

PhysicalFileSystem::PhysicalFileSystem(Disk& disk) : disk(disk) {}
Disk& PhysicalFileSystem::GetDisk(void) {
    return disk;
}