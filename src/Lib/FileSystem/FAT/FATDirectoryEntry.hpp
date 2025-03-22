#ifndef MathLib_FileSystem_FAT_DirectoryEntry_H
#define MathLib_FileSystem_FAT_DirectoryEntry_H
#include <stdint.h>

namespace MathLib {
    struct FATDirectoryEntry {
        static constexpr char unusedEntry = 0xeb;
        char name[11];
        bool readOnly : 1;
        bool hidden : 1;
        bool system : 1;
        bool volumeID : 1;
        bool directory : 1;
        bool archive : 1;
        uint16_t resrved : 10;
        uint8_t creationTime;
        uint32_t time;
        uint16_t lastAccessedDate;
        uint16_t highCluster;
        uint32_t lastModificationTime;
        uint16_t lowCluster;
        uint32_t size;

        [[nodiscard]] bool IsLongFileName(void) const;
        [[nodiscard]] String GetName(void) const;
        [[nodiscard]] uint32_t GetCluster(void) const;
        void SetCluster(uint32_t cluster);
    } __attribute__((packed));
}

#endif