#ifndef Freestanding
#include "HostDisk.hpp"
#include "../Host.hpp"

HostDisk::~HostDisk(void) {
    if (fclose(file)) Panic("Failed to close file");
}
HostDisk::HostDisk(String path, size_t size_, size_t sectorSize_) : size(size_ + size_ % sectorSize_), sectorSize(sectorSize_), file(fopen(path.ToString(), "w")) {
    if (!file) Panic("No file specified");
    uint8_t zero = 0;
    for (size_t i = 0; i < size; i++)
        if (fwrite(&zero, sizeof(uint8_t), 1, file) != 1) Panic("Failed to write placeholder data for disk");
    if (fflush(file)) Panic("Failed to flush disk");
}
size_t HostDisk::GetSectorSize(void) const {
    return sectorSize;
}
size_t HostDisk::GetSize(void) const {
    return size;
}
bool HostDisk::Flush(void) {
    return !fflush(file);
}
bool HostDisk::ReadSectors(size_t sector, void* buff, size_t count) {
    return fseek(file, sector * sectorSize, SEEK_SET) ? false : fread(buff, sectorSize, count, file) == count;
}
bool HostDisk::WriteSectors(size_t sector, const void* buff, size_t count) {
    return fseek(file, sector * sectorSize, SEEK_SET) ? false : fwrite(buff, sectorSize, count, file) == count;
}

#endif