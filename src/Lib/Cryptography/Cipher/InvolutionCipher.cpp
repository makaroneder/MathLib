#include "InvolutionCipher.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    Array<uint8_t> InvolutionCipher::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        return Convert(data, key, SIZE_MAX);    
    }
    Array<uint8_t> InvolutionCipher::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        const size_t min = range.GetMin();
        const size_t max = range.GetMax();
        if (min >= max) return Array<uint8_t>();
        const Array<uint8_t> ret = Convert(data, key, max);
        const size_t size = ret.GetSize();
        return min < size ? CollectionToArray<uint8_t>(SubSequence<uint8_t>(ret, Interval<size_t>(min, size))) : Array<uint8_t>();
    }
}