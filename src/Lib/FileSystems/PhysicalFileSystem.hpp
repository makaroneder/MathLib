#ifndef PhysicalFileSystem_H
#define PhysicalFileSystem_H
#include "FileSystem.hpp"
#include "../Interfaces/ByteDevice.hpp"

struct PhysicalFileSystem : FileSystem {
    PhysicalFileSystem(ByteDevice& disk);
    ByteDevice& GetDisk(void);

    protected:
    ByteDevice& disk;
};

#endif