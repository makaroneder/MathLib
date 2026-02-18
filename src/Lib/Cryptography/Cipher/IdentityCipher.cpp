#include "IdentityCipher.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    Array<uint8_t> IdentityCipher::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        return key.IsEmpty() ? CollectionToArray(data) : Array<uint8_t>();
    }
    Array<uint8_t> IdentityCipher::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(data.GetSize(), range.GetMax());
        return start < end && key.IsEmpty() ? CollectionToArray(SubSequence<uint8_t>(data, Interval<size_t>(start, end))) : Array<uint8_t>();
    }
}