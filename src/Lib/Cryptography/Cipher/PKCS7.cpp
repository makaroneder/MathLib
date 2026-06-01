#include "PKCS7.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"
#include "../../Interfaces/Sequence/ExtendedSequence.hpp"

namespace MathLib {
    Array<uint8_t> PKCS7::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (key.type != CipherKey::Type::Normal || key.data.GetSize() != sizeof(uint8_t)) return Array<uint8_t>();
        const uint8_t length = key.data.AsT<uint8_t>().Get();
        const size_t size = data.GetSize();
        uint8_t n = 1;
        while ((size + n) % length) n++;
        return CollectionToArray<uint8_t>(ExtendedSequence<uint8_t>(data, n, n));
    }
    Array<uint8_t> PKCS7::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        const size_t min = range.GetMin();
        const size_t max = range.GetMax();
        if (min >= max) return Array<uint8_t>();
        if (key.type != CipherKey::Type::Normal || key.data.GetSize() != sizeof(uint8_t)) return Array<uint8_t>();
        const uint8_t paddingLength = key.data.AsT<uint8_t>().Get();
        const size_t size = data.GetSize();
        if (size <= paddingLength) return Array<uint8_t>();
        const size_t end = Min<size_t>(max, size - paddingLength);
        return min < end ? CollectionToArray<uint8_t>(SubSequence<uint8_t>(data, Interval<size_t>(min, end))) : Array<uint8_t>();
    }
}