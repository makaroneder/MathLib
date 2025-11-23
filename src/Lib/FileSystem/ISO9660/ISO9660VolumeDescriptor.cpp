#include "ISO9660VolumeDescriptor.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    ISO9660VolumeDescriptor::ISO9660VolumeDescriptor(void) : type(Type::VolumeDescriptorSetTerminator), signature(), version(0) {}
    bool ISO9660VolumeDescriptor::IsValid(void) const {
        for (uint8_t i = 0; i < SizeOfArray(signature); i++)
            if (signature[i] != expectedSignature[i]) return false;
        return version == expectedVersion;
    }
}