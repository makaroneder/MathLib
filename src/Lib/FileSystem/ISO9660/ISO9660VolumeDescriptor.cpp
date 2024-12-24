#include "ISO9660VolumeDescriptor.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    bool ISO9660VolumeDescriptor::IsValid(void) const {
        StartBenchmark
        for (uint8_t i = 0; i < SizeOfArray(signature); i++)
            if (signature[i] != expectedSignature[i]) ReturnFromBenchmark(false);
        ReturnFromBenchmark(version == expectedVersion);
    }
}