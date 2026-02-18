#include "Ar.hpp"
#include "ArHeader.hpp"
#include "../../../Interfaces/Sequence/ByteArray.hpp"

namespace MathLib {
    Array<uint8_t> Ar::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (!key.IsEmpty()) return Array<uint8_t>();
        const size_t size = data.GetSize();
        ByteArray ret = ByteArray(size + sizeof(ArHeader));
        if (!ret.Write<ArHeader>(ArHeader(path, size))) return Array<uint8_t>();
        uint8_t buff[size];
        for (size_t i = 0; i < size; i++) buff[i] = data.At(i);
        return ret.WriteBuffer(buff, size) && (!(size % 2) || ret.Write<char>('\n')) ? ret.GetArray() : Array<uint8_t>();
    }
    bool Ar::IsValidHeader(Readable& readable, const CipherKey& key) const {
        uint64_t signature = 0;
        if (!key.IsEmpty() || !readable.Read<uint64_t>(signature)) return false;
        return signature == *(const uint64_t*)signature1 || signature == *(const uint64_t*)signature2;
    }
    bool Ar::IsValidFooter(Readable&, const CipherKey& key) const {
        return key.IsEmpty();
    }
    bool Ar::Create(Writable& writable, const CipherKey& key) const {
        return key.IsEmpty() && writable.Puts(signature1);
    }
    bool Ar::UpdateHeaderAndFooter(ByteDevice&, const CipherKey& key) const {
        return key.IsEmpty();
    }
    FileCipherData Ar::Identify(Readable& readable, const CipherKey& key, size_t& unpaddedSize) const {
        if (!key.IsEmpty()) return FileCipherData();
        ArHeader header;
        if (!readable.Read(header) || !header.IsValid()) return FileCipherData();
        const String name = header.GetName();
        unpaddedSize = header.GetSize();
        return FileCipherData(name.IsEmpty() ? FileCipherData::Type::Skip : FileCipherData::Type::Normal, name, unpaddedSize + sizeof(ArHeader) + unpaddedSize % 2);
    }
}