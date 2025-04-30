#include "TARHeader.hpp"
#include <ExternArray.hpp>
#include <String.hpp>
#include <Memory.hpp>

bool TARHeader::IsValid(void) const {
    const size_t end = SizeOfArray(checksum) - 2;
    if (checksum[end] != '\0' || checksum[end + 1] != ' ') return false;
    char tmp[end];
    MathLib::MemoryCopy(checksum, tmp, end);
    return GetChecksum() == MathLib::StringToNumber(MathLib::ExternArray<char>(tmp, end), 8);
}
size_t TARHeader::GetChecksum(void) const {
    const uint8_t* this8 = (const uint8_t*)this;
    uint8_t buff[512];
    for (uint16_t i = 0; i < 512; i++) buff[i] = this8[i];
    TARHeader* other = (TARHeader*)buff;
    for (uint8_t i = 0; i < SizeOfArray(checksum); i++) other->checksum[i] = ' ';
    size_t ret = 0;
    for (uint16_t i = 0; i < 512; i++) ret += buff[i];
    return ret;
}
MathLib::String TARHeader::GetName(void) const {
    return path;
}
size_t TARHeader::GetSize(void) const {
    char tmp[SizeOfArray(size) - 1];
    MathLib::MemoryCopy(size, tmp, SizeOfArray(tmp));
    return MathLib::StringToNumber(MathLib::ExternArray<char>(tmp, SizeOfArray(tmp)), 8);
}
size_t TARHeader::PaddSize(size_t size) {
    const size_t rem = size % 512;
    return rem ? size + 1024 - rem : size + 512;
}
bool TARHeader::Create(const MathLib::Sequence<char>& path_, size_t dataSize) {
    type = Type::Normal;
    uint8_t end = path_.GetSize();
    for (uint8_t i = 0; i < SizeOfArray(path); i++) path[i] = i < end ? path_[i] : '\0';
    end = SizeOfArray(size) - 1;
    const MathLib::String sizeStr = MathLib::ToString(dataSize, 8, end);
    if (sizeStr.GetSize() != end) return false;
    for (uint8_t i = 0; i < end; i++) size[i] = sizeStr.At(i);
    size[end] = '\0';
    end = SizeOfArray(mode) - 1;
    for (uint8_t i = 0; i < end; i++)
        mode[i] = ownerID[i] = groupID[i] = '0';
    for (uint8_t i = 0; i < 3; i++) mode[end - i - 1] = '6';
    mode[end] = ownerID[end] = groupID[end] = '\0';
    for (uint8_t i = 0; i < SizeOfArray(lastModificationTime); i++) lastModificationTime[i] = '\0';
    for (uint8_t i = 0; i < SizeOfArray(linkedPath); i++) linkedPath[i] = '\0';
    end = SizeOfArray(checksum) - 2;
    const MathLib::String checksumStr = MathLib::ToString(GetChecksum(), 8, end);
    if (checksumStr.GetSize() != end) return false;
    for (uint8_t i = 0; i < end; i++) checksum[i] = checksumStr.At(i);
    checksum[end] = '\0';
    checksum[end + 1] = ' ';
    return true;
}