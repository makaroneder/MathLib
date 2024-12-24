#ifndef MathLib_FileSystem_PhysicalFileSystem_H
#define MathLib_FileSystem_PhysicalFileSystem_H
#include "FileSystem.hpp"
#include "../Interfaces/ByteDevice.hpp"

namespace MathLib {
    struct PhysicalFileSystem : FileSystem {
        PhysicalFileSystem(ByteDevice& disk);
        ByteDevice& GetDisk(void);
        virtual bool Create(void) = 0;

        protected:
        ByteDevice& disk;
    };
}

#endif