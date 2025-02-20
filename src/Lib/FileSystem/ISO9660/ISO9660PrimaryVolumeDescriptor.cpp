#include "ISO9660PrimaryVolumeDescriptor.hpp"

namespace MathLib {
    bool ISO9660PrimaryVolumeDescriptor::IsValid(void) const {
        StartAndReturnFromBenchmark (
            ISO9660VolumeDescriptor::IsValid() &&
            type == Type::PrimaryVolumeDescriptor &&
            bytesPerSector.little == 2048 &&
            creationDate.IsValid() &&
            modificationDate.IsValid() &&
            expirationDate.IsValid() &&
            effectiveDate.IsValid()
        );
    }
}