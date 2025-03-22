#include "ZLibHeader.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    ZLibHeader::ZLibHeader(void) : compressionMethod(0), compressionInfo(0), checkBits(0), dictionary(false), level(0) {
        EmptyBenchmark
    }
}