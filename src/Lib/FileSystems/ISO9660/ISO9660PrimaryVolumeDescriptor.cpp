#include "ISO9660PrimaryVolumeDescriptor.hpp"

bool ISO9660PrimaryVolumeDescriptor::IsValid(void) const {
    return (
        ISO9660VolumeDescriptor::IsValid() &&
        type == Type::PrimaryVolumeDescriptor &&
        bytesPerSector.little == 2048 &&
        creationDate.IsValid() &&
        modificationDate.IsValid() &&
        expirationDate.IsValid() &&
        effectiveDate.IsValid()
    );
}