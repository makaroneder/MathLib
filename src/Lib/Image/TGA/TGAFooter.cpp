#include "TGAFooter.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    TGAFooter::TGAFooter(void) : extensionOffset(0), developerOffset(0) {}
    TGAFooter::TGAFooter(uint32_t extensionOffset, uint32_t developerOffset) : extensionOffset(extensionOffset), developerOffset(developerOffset) {
        MemoryCopy(expectedSignature, signature, SizeOfArray(signature));
    }
}