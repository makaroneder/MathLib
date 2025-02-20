#include "MathFSBootSector.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    MathFSBootSector::MathFSBootSector(void) : signature1(0), root(0), signature2(0) {
        EmptyBenchmark
    }
    MathFSBootSector::MathFSBootSector(uint64_t root) : signature1(expectedSignature1), root(root), signature2(expectedSignature2) {
        EmptyBenchmark
    }
    bool MathFSBootSector::IsValid(void) const {
        StartAndReturnFromBenchmark(signature1 == expectedSignature1 && signature2 == expectedSignature2);
    }
}