#include "TAR.hpp"
#include "TARHeader.hpp"
#include <Interfaces/ByteArray.hpp>

bool TAR::IsValidHeader(MathLib::Readable& device, const MathLib::Sequence<uint64_t>& key) const {
    (void)device;
    return key.IsEmpty();
}
bool TAR::IsValidFooter(MathLib::Readable& device, const MathLib::Sequence<uint64_t>& key) const {
    if (!key.IsEmpty()) return false;
    for (uint8_t i = 0; i < 128; i++)
        if (device.Read<uint64_t>().GetOr(UINT64_MAX)) return false;
    return true;
}
bool TAR::Create(MathLib::Writable& device, const MathLib::Sequence<uint64_t>& key) const {
    if (!key.IsEmpty()) return false;
    for (uint8_t i = 0; i < 128; i++)
        if (!device.Write<uint64_t>(0)) return false;
    return true;
}
bool TAR::UpdateHeaderAndFooter(MathLib::ByteDevice& device, const MathLib::Sequence<uint64_t>& key) const {
    if (!key.IsEmpty()) return false;
    while (true) {
        const size_t pos = device.Tell();
        const FileCipherData data = IdentityFileCipher::Identify(device, key);
        if (data.type == FileCipherData::Type::Invalid) {
            if (!device.Seek(pos, MathLib::SeekMode::Set)) return false;
            break;
        }
        if (!device.Seek(pos + data.size, MathLib::SeekMode::Set)) return false;
    }
    return Create(device, key);
}
FileCipherData TAR::Identify(MathLib::Readable& device, const MathLib::Sequence<uint64_t>& key, size_t& unpaddedSize) const {
    if (!key.IsEmpty()) return FileCipherData();
    uint8_t buff[512];
    if (!device.ReadBuffer(buff, SizeOfArray(buff))) return FileCipherData();
    const TARHeader* header = (const TARHeader*)buff;
    if (!header->IsValid()) return FileCipherData();
    unpaddedSize = header->GetSize();
    return FileCipherData(header->type == TARHeader::Type::Normal ? FileCipherData::Type::Normal : FileCipherData::Type::Skip, header->GetName(), TARHeader::PaddSize(unpaddedSize));
}
MathLib::Array<uint8_t> TAR::Encrypt(const MathLib::Sequence<uint8_t>& data, const MathLib::Sequence<uint64_t>& key) const {
    if (key.GetSize() != 1) return MathLib::Array<uint8_t>();
    const size_t dataSize = data.GetSize();
    uint8_t headerBuff[512] = { 0, };
    TARHeader* header = (TARHeader*)headerBuff;
    if (!header->Create(MathLib::String((const char*)key.At(0)), dataSize)) return MathLib::Array<uint8_t>();
    size_t size = dataSize;
    const size_t rem = size % 512;
    if (rem) size += 512 - rem;
    MathLib::ByteArray ret = MathLib::ByteArray(size + SizeOfArray(headerBuff));
    if (!ret.WriteBuffer(headerBuff, SizeOfArray(headerBuff))) return MathLib::Array<uint8_t>();
    uint8_t buff[size];
    for (size_t i = 0; i < dataSize; i++) buff[i] = data.At(i);
    for (size_t i = dataSize; i < size; i++) buff[i] = 0;
    return ret.WriteBuffer(buff, size) ? ret.GetArray() : MathLib::Array<uint8_t>();
}