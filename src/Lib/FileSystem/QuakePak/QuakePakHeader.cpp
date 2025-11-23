#include "QuakePakHeader.hpp"
#include "QuakePakFileEntry.hpp"

namespace MathLib {
    QuakePakHeader::QuakePakHeader(void) : fileTablePosition(0), fileTableSize(0) {}
    QuakePakHeader::QuakePakHeader(uint32_t fileTablePosition) : fileTablePosition(fileTablePosition), fileTableSize(0) {
        MemoryCopy(expectedSignature, signature, SizeOfArray(signature));
    }
    bool QuakePakHeader::IsValid(void) const {
        return *(const uint32_t*)signature == *(const uint32_t*)expectedSignature && !(fileTableSize % sizeof(QuakePakFileEntry));
    }
}