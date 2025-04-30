#include "GNUTAR.hpp"
#include "GNUTARHeader.hpp"
#include <Interfaces/ByteArray.hpp>

FileCipherData GNUTAR::Identify(MathLib::Readable& device, const MathLib::Sequence<uint64_t>& key, size_t& unpaddedSize) const {
    if (!key.IsEmpty()) return FileCipherData();
    uint8_t buff[512];
    if (!device.ReadBuffer(buff, SizeOfArray(buff))) return FileCipherData();
    // TODO: Handle multi volume GNU archive
    const GNUTARHeader* header = (const GNUTARHeader*)buff;
    if (!header->IsValid()) return FileCipherData();
    unpaddedSize = header->GetSize();
    return FileCipherData(header->type == TARHeader::Type::Normal || header->type == TARHeader::Type::GNUNormal ? FileCipherData::Type::Normal : FileCipherData::Type::Skip, header->GetName(), GNUTARHeader::PaddSize(unpaddedSize));
}
MathLib::Array<uint8_t> GNUTAR::Encrypt(const MathLib::Sequence<uint8_t>& data, const MathLib::Sequence<uint64_t>& key) const {
    if (key.GetSize() != 1) return MathLib::Array<uint8_t>();
    const size_t dataSize = data.GetSize();
    GNUTARHeader header;
    if (!header.Create(MathLib::String((const char*)key.At(0)), dataSize)) return MathLib::Array<uint8_t>();
    size_t size = dataSize;
    const size_t rem = size % 512;
    if (rem) size += 512 - rem;
    MathLib::ByteArray ret = MathLib::ByteArray(size + sizeof(GNUTARHeader));
    if (!ret.Write<GNUTARHeader>(header)) return MathLib::Array<uint8_t>();
    uint8_t buff[size];
    for (size_t i = 0; i < dataSize; i++) buff[i] = data.At(i);
    for (size_t i = dataSize; i < size; i++) buff[i] = 0;
    return ret.WriteBuffer(buff, size) ? ret.GetArray() : MathLib::Array<uint8_t>();
}