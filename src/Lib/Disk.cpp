#include "Disk.hpp"

Disk::~Disk(void) {}
bool Disk::Read(void* buffer, size_t size) {
    const size_t sectorSize = GetSectorSize();
    const size_t start = position % sectorSize;
    const size_t count = size / sectorSize + (size % sectorSize != 0) + (start != 0);
    uint8_t tmp[count * sectorSize];
    if (!ReadSectors(position / sectorSize, tmp, count)) return false;
    uint8_t* buff8 = (uint8_t*)buffer;
    for (size_t i = 0; i < size; i++) buff8[i] = tmp[i + start];
    position += size;
    return true;
}
bool Disk::Write(const void* buffer, size_t size) {
    const size_t sectorSize = GetSectorSize();
    const size_t start = position % sectorSize;
    const size_t count = size / sectorSize + (size % sectorSize != 0) + (start != 0);
    uint8_t tmp[count * sectorSize];
    if (!ReadSectors(position / sectorSize, tmp, count)) return false;
    const uint8_t* buff8 = (const uint8_t*)buffer;
    for (size_t i = 0; i < size; i++) tmp[i + start] = buff8[i];
    if (!WriteSectors(position / sectorSize, tmp, count)) return false;
    position += size;
    return true;
}
bool Disk::Seek(ssize_t offset, SeekMode mode) {
    switch (mode) {
        case SeekMode::Set: {
            if (offset < 0) return false;
            position = offset;
            break;
        }
        case SeekMode::Current: {
            position += offset;
            break;
        }
        case SeekMode::End: {
            if (offset < 0) return false;
            position = GetSize() - offset;
            break;
        }
    }
    return true;
}
size_t Disk::Tell(void) {
    return position;
}