#include "VirtualDisk.hpp"
#include "Host.hpp"

VirtualDisk::VirtualDisk(File& file, const size_t& size_, const size_t& sectorSize_) : size(size_ + size_ % sectorSize_), sectorSize(sectorSize_), file(file) {
    for (size_t i = 0; i < size; i++)
        if (!file.Write<uint8_t>(0)) Panic("Failed to write placeholder data for disk");
}
size_t VirtualDisk::GetSectorSize(void) const {
    return sectorSize;
}
size_t VirtualDisk::GetSize(void) const {
    return size;
}
bool VirtualDisk::ReadSectors(const size_t& sector, void* buff, const size_t& count) {
    return file.Seek(sector * sectorSize, SeekMode::Set) ? false : file.ReadBuffer(buff, sectorSize * count);
}
bool VirtualDisk::WriteSectors(const size_t& sector, const void* buff, const size_t& count) {
    return file.Seek(sector * sectorSize, SeekMode::Set) ? false : file.WriteBuffer(buff, sectorSize * count);
}