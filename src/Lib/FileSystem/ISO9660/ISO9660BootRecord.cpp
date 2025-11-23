#include "ISO9660BootRecord.hpp"

namespace MathLib {
    ISO9660BootRecord::ISO9660BootRecord(void) : ISO9660VolumeDescriptor(), systemIdentifier(), bootIdentifier(), elToritoBootCatalog(0), reserved() {}
}