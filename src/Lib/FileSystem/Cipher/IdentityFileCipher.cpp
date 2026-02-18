#include "IdentityFileCipher.hpp"
#include "../../Interfaces/Sequence/ByteArray.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    FileCipherData IdentityFileCipher::Identify(Readable& device, const CipherKey& key) const {
        size_t unpaddedSize = 0;
        return Identify(device, key, unpaddedSize);
    }
    Array<uint8_t> IdentityFileCipher::DecryptReadablePartial(Readable& readable, const CipherKey& key, const Interval<size_t>& range) const {
        size_t unpaddedSize = 0;
        const FileCipherData header = Identify(readable, key, unpaddedSize);
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(range.GetMax(), unpaddedSize);
        if (start >= end || !readable.Skip(start)) return Array<uint8_t>();
        const size_t size = end - start;
        Array<uint8_t> ret = Array<uint8_t>(size);
        return readable.ReadBuffer(ret.GetValue(), size) ? ret : Array<uint8_t>();
    }
}