#include "XZ.hpp"
#include "XZIndex.hpp"
#include "XZHeader.hpp"
#include "XZFooter.hpp"
#include "../../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    Array<uint8_t> XZ::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (!key.IsEmpty()) return Array<uint8_t>();
        ByteArray ret;
        const XZFlags flags = XZFlags(XZFlags::CheckType::None);
        const XZHeader header = XZHeader(flags);
        if (!ret.Write<XZHeader>(header)) return Array<uint8_t>();
        uint8_t paddingSize;
        const Array<uint8_t> compressed = XZRecord().Get(data, (XZFlags::CheckType)flags.checkType, paddingSize);
        if (!ret.WriteCollection<uint8_t>(compressed)) return Array<uint8_t>();
        const size_t tmp = ret.Tell();
        const XZIndex index = MakeArray<XZRecord>(XZRecord(compressed.GetSize() - paddingSize, data.GetSize()));
        if (!index.Save(ret)) return Array<uint8_t>();
        const XZFooter footer = XZFooter(ret.Tell() - tmp, flags);
        if (!ret.Write<XZFooter>(footer)) return Array<uint8_t>();
        return ret.GetArray();
    }
    Array<uint8_t> XZ::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        if (!key.IsEmpty()) return Array<uint8_t>();
        ByteArray readable = data;
        XZHeader header;
        if (!readable.Read<XZHeader>(header) || !header.IsValid()) return Array<uint8_t>();
        const size_t position = readable.Tell();
        if (!readable.Seek(sizeof(uint16_t) * 2 + sizeof(uint32_t) * 2, SeekMode::End)) return Array<uint8_t>();
        XZFooter footer;
        if (!readable.Read<XZFooter>(footer) || !footer.IsValid() || footer.flags != header.flags) return Array<uint8_t>();
        if (!readable.Seek(sizeof(uint16_t) * 2 + sizeof(uint32_t) * 2 + footer.GetIndexSize(), SeekMode::End)) return Array<uint8_t>();
        XZIndex index;
        if (!index.Load(readable)) return Array<uint8_t>();
        if (!readable.Seek(position, SeekMode::Set)) return Array<uint8_t>();
        Array<uint8_t> ret;
        for (const XZRecord& record : index.records) {
            if (!ret.AddSequence(record.DecryptReadable(readable, CipherKey(MakeArray<CipherKey>(
                CipherKey(ByteArray::ToByteArray<XZFlags::CheckType>((XZFlags::CheckType)header.flags.checkType))
            ))))) return Array<uint8_t>();
        }
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(range.GetMax(), ret.GetSize());
        return start < end ? CollectionToArray<uint8_t>(SubSequence<uint8_t>(ret, Interval<size_t>(start, end))) : Array<uint8_t>();
    }
}