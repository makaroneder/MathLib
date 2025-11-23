#include "UnixStandardTAR.hpp"
#include "UnixStandardTARHeader.hpp"
#include "../../../../Interfaces/Sequence/ByteArray.hpp"

namespace MathLib {
    FileCipherData UnixStandardTAR::Identify(Readable& device, const Sequence<uint64_t>& key, size_t& unpaddedSize) const {
        if (!key.IsEmpty()) return FileCipherData();
        uint8_t buff[512];
        if (!device.ReadBuffer(buff, SizeOfArray(buff))) return FileCipherData();
        const UnixStandardTARHeader* header = (const UnixStandardTARHeader*)buff;
        if (!header->IsValid()) return FileCipherData();
        unpaddedSize = header->GetSize();
        return FileCipherData(header->type == TARHeader::Type::Normal ? FileCipherData::Type::Normal : FileCipherData::Type::Skip, header->GetName(), UnixStandardTARHeader::PaddSize(unpaddedSize));
    }
    Array<uint8_t> UnixStandardTAR::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        if (key.GetSize() > 1) return Array<uint8_t>();
        const size_t dataSize = data.GetSize();
        UnixStandardTARHeader header;
        if (!header.Create(key.IsEmpty() ? "" : String((const char*)key.At(0)), dataSize)) return Array<uint8_t>();
        size_t size = dataSize;
        const size_t rem = size % 512;
        if (rem) size += 512 - rem;
        ByteArray ret = ByteArray(size + sizeof(UnixStandardTARHeader));
        if (!ret.Write<UnixStandardTARHeader>(header)) return Array<uint8_t>();
        uint8_t buff[size];
        for (size_t i = 0; i < dataSize; i++) buff[i] = data.At(i);
        for (size_t i = dataSize; i < size; i++) buff[i] = 0;
        return ret.WriteBuffer(buff, size) ? ret.GetArray() : Array<uint8_t>();
    }
}