#include "IdentityFileCipher.hpp"
#include <Interfaces/ByteArray.hpp>
#include <Interfaces/SubSequence.hpp>

FileCipherData IdentityFileCipher::Identify(MathLib::Readable& device, const MathLib::Sequence<uint64_t>& key) const {
    size_t unpaddedSize = 0;
    return Identify(device, key, unpaddedSize);
}
MathLib::Array<uint8_t> IdentityFileCipher::DecryptReadablePartial(MathLib::Readable& readable, const MathLib::Sequence<uint64_t>& key, const MathLib::Interval<size_t>& range) const {
    size_t unpaddedSize = 0;
    const FileCipherData header = Identify(readable, key, unpaddedSize);
    const size_t start = range.GetMin();
    const size_t end = MathLib::Min<size_t>(range.GetMax(), unpaddedSize);
    if (start >= end || !readable.Skip(start)) return MathLib::Array<uint8_t>();
    const size_t size = end - start;
    MathLib::Array<uint8_t> ret = MathLib::Array<uint8_t>(size);
    if (!readable.ReadBuffer(ret.GetValue(), size)) return MathLib::Array<uint8_t>();
    return ret;
}