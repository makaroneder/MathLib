#ifndef PhysicalFileSystem_H
#define PhysicalFileSystem_H
#include "FileSystem.hpp"
#include "Disk.hpp"

struct PhysicalFileSystem : FileSystem {
    PhysicalFileSystem(Disk& disk);
    Disk& GetDisk(void);

    protected:
    Disk& disk;
};

#endif