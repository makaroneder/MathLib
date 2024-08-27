#include "VolumeDescriptor.hpp"
#include "../../Typedefs.hpp"

bool VolumeDescriptor::IsValid(void) const {
    for (uint8_t i = 0; i < SizeOfArray(signature); i++)
        if (signature[i] != expectedSignature[i]) return false;
    return version == expectedVersion;
}