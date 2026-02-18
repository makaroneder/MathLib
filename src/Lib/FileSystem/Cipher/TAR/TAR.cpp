#include "TAR.hpp"
#include "TARHeader.hpp"
#include "../../../Interfaces/Sequence/ByteArray.hpp"

namespace MathLib {
    bool TAR::IsValidHeader(Readable&, const CipherKey& key) const {
        return key.IsEmpty();
    }
    bool TAR::IsValidFooter(Readable& device, const CipherKey& key) const {
        if (!key.IsEmpty()) return false;
        for (uint8_t i = 0; i < 128; i++)
            if (device.Read<uint64_t>().GetOr(UINT64_MAX)) return false;
        return true;
    }
    bool TAR::Create(Writable& device, const CipherKey& key) const {
        if (!key.IsEmpty()) return false;
        for (uint8_t i = 0; i < 128; i++)
            if (!device.Write<uint64_t>(0)) return false;
        return true;
    }
    bool TAR::UpdateHeaderAndFooter(ByteDevice& device, const CipherKey& key) const {
        if (!key.IsEmpty()) return false;
        while (true) {
            const size_t pos = device.Tell();
            const FileCipherData data = IdentityFileCipher::Identify(device, key);
            if (data.type == FileCipherData::Type::Invalid) {
                if (!device.Seek(pos, SeekMode::Set)) return false;
                break;
            }
            if (!device.Seek(pos + data.size, SeekMode::Set)) return false;
        }
        return Create(device, key);
    }
    FileCipherData TAR::Identify(Readable& device, const CipherKey& key, size_t& unpaddedSize) const {
        if (!key.IsEmpty()) return FileCipherData();
        uint8_t buff[512];
        if (!device.ReadBuffer(buff, SizeOfArray(buff))) return FileCipherData();
        const TARHeader* header = (const TARHeader*)buff;
        if (!header->IsValid()) return FileCipherData();
        unpaddedSize = header->GetSize();
        return FileCipherData(header->type == TARHeader::Type::Normal ? FileCipherData::Type::Normal : FileCipherData::Type::Skip, header->GetName(), TARHeader::PaddSize(unpaddedSize));
    }
    Array<uint8_t> TAR::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (!key.IsEmpty()) return Array<uint8_t>();
        const size_t dataSize = data.GetSize();
        uint8_t headerBuff[512] = { 0, };
        TARHeader* header = (TARHeader*)headerBuff;
        if (!header->Create(path, dataSize)) return Array<uint8_t>();
        size_t size = dataSize;
        const size_t rem = size % 512;
        if (rem) size += 512 - rem;
        ByteArray ret = ByteArray(size + SizeOfArray(headerBuff));
        if (!ret.WriteBuffer(headerBuff, SizeOfArray(headerBuff))) return Array<uint8_t>();
        uint8_t buff[size];
        for (size_t i = 0; i < dataSize; i++) buff[i] = data.At(i);
        for (size_t i = dataSize; i < size; i++) buff[i] = 0;
        return ret.WriteBuffer(buff, size) ? ret.GetArray() : Array<uint8_t>();
    }
}