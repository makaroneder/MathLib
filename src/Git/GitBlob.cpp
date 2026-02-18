#include "GitBlob.hpp"

MathLib::Array<uint8_t> GitBlob::Encrypt(const MathLib::Sequence<uint8_t>& data, const MathLib::CipherKey& key) const {
    if (!key.IsEmpty()) return MathLib::Array<uint8_t>();
    const size_t size = data.GetSize();
    const MathLib::String str = MathLib::String(blobStart) + MathLib::ToString(size, 10);
    MathLib::Array<uint8_t> ret = MathLib::Array<uint8_t>((const uint8_t*)str.GetValue(), str.GetSize() + 1);
    if (!ret.AddSequence(data)) return MathLib::Array<uint8_t>();
    return ret;
}
MathLib::Array<uint8_t> GitBlob::DecryptReadablePartial(MathLib::Readable& readable, const MathLib::CipherKey& key, const MathLib::Interval<size_t>& range) const {
    if (!key.IsEmpty()) return MathLib::Array<uint8_t>();
    char startStr[6];
    if (!readable.ReadBuffer(startStr, SizeOfArray(startStr) - 1)) return MathLib::Array<uint8_t>();
    startStr[5] = '\0';
    if (MathLib::String(blobStart) != startStr) return MathLib::Array<uint8_t>();
    MathLib::String sizeStr;
    while (true) {
        char tmp;
        if (!readable.Read<char>(tmp)) return MathLib::Array<uint8_t>();
        if (!tmp) break;
        sizeStr += tmp;
    }
    const size_t size = MathLib::StringToNumber(sizeStr);
    const size_t start = range.GetMin();
    const size_t end = MathLib::Min<size_t>(range.GetMax(), size);
    if (end <= start || !readable.Skip(start)) return MathLib::Array<uint8_t>();
    const size_t readSize = end - start;
    MathLib::Array<uint8_t> ret = MathLib::Array<uint8_t>(readSize);
    if (!readable.ReadBuffer(ret.GetValue(), readSize)) return MathLib::Array<uint8_t>();
    return ret;
}