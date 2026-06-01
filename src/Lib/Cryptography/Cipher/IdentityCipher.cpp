#include "IdentityCipher.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    Array<uint8_t> IdentityCipher::Convert(const Sequence<uint8_t>& data, const CipherKey& key, size_t limit) const {
        const size_t end = Min<size_t>(data.GetSize(), limit);
        return key.IsEmpty() ? CollectionToArray(SubSequence<uint8_t>(data, Interval<size_t>(0, end))) : Array<uint8_t>();
    }
}