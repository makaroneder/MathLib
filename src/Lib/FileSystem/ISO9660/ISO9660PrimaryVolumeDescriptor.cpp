#include "ISO9660PrimaryVolumeDescriptor.hpp"

namespace MathLib {
    ISO9660PrimaryVolumeDescriptor::ISO9660PrimaryVolumeDescriptor(void) : ISO9660VolumeDescriptor(), reserved1(0), systemIdentifier(), volumeIdentifier(), reserved2(0), sectors(), sizeOfSet(), volumeNumber(), bytesPerSector(), bytesPerPathTable(), pathTable(), root(), volumeSetIdentifier(), publisherIdentifier(), dataPreparerIdentifier(), applicationIdentifier(), copyrightPath(), abstractPath(), bibliographicPath(), creationDate(), modificationDate(), expirationDate(), effectiveDate(), fileStructureVersion(0), reserved4(0), osSpecific(), reserved5() {}
    bool ISO9660PrimaryVolumeDescriptor::IsValid(void) const {
        return ISO9660VolumeDescriptor::IsValid() && type == Type::PrimaryVolumeDescriptor && bytesPerSector.little == 2048;
    }
}