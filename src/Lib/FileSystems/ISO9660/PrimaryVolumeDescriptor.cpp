#include "PrimaryVolumeDescriptor.hpp"

bool PrimaryVolumeDescriptor::IsValid(void) const {
    if (!VolumeDescriptor::IsValid()) return false;
    return type == Type::PrimaryVolumeDescriptor && bytesPerSector.little == 2048;
}