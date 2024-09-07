#include "PrimaryVolumeDescriptor.hpp"

bool PrimaryVolumeDescriptor::IsValid(void) const {
    return (
        VolumeDescriptor::IsValid() &&
        type == Type::PrimaryVolumeDescriptor &&
        bytesPerSector.little == 2048 &&
        creationDate.IsValid() &&
        modificationDate.IsValid() &&
        expirationDate.IsValid() &&
        effectiveDate.IsValid()
    );
}