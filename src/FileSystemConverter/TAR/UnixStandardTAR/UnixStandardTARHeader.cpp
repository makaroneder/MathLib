#include "UnixStandardTARHeader.hpp"

bool UnixStandardTARHeader::IsValid(void) const {
    return ExtendedTARHeader::IsValid(expectedSignature, expectedVersion);
}
MathLib::String UnixStandardTARHeader::GetName(void) const {
    const MathLib::String name = TARHeader::GetName();
    return pathPrefix[0] ? MathLib::String(pathPrefix) + '/' + name : name;
}
bool UnixStandardTARHeader::Create(const MathLib::Sequence<char>& path_, size_t dataSize) {
    for (uint8_t i = 0; i < SizeOfArray(pathPrefix); i++) pathPrefix[i] = '\0';
    for (uint8_t i = 0; i < SizeOfArray(reserved); i++) reserved[i] = 0;
    return ExtendedTARHeader::Create(path_, dataSize, expectedSignature, expectedVersion);
}