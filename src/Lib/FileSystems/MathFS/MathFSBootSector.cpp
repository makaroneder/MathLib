#include "MathFSBootSector.hpp"

MathFSBootSector::MathFSBootSector(void) : signature1(0), root(0), signature2(0) {}
MathFSBootSector::MathFSBootSector(uint64_t root) : signature1(expectedSignature1), root(root), signature2(expectedSignature2) {}
bool MathFSBootSector::IsValid(void) const {
    return signature1 == expectedSignature1 && signature2 == expectedSignature2;
}