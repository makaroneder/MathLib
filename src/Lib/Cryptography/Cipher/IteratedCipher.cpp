#include "IteratedCipher.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    IteratedCipher::IteratedCipher(Cipher& cipher) : cipher(cipher) {}
    Array<uint8_t> IteratedCipher::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        if (key.IsEmpty()) return Array<uint8_t>();
        Array<uint8_t> ret = CollectionToArray<uint8_t>(data);
        const size_t iters = key.At(0);
        const SubSequence<uint64_t> subKey = SubSequence<uint64_t>(key, Interval<size_t>(1, key.GetSize()));
        for (size_t i = 0; i < iters; i++) ret = cipher.Encrypt(ret, subKey);
        return ret;
    }
    Array<uint8_t> IteratedCipher::DecryptPartial(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, const Interval<size_t>& range) const {
        if (key.IsEmpty()) return Array<uint8_t>();
        Array<uint8_t> ret = CollectionToArray<uint8_t>(data);
        const size_t iters = key.At(0);
        const SubSequence<uint64_t> subKey = SubSequence<uint64_t>(key, Interval<size_t>(1, key.GetSize()));
        for (size_t i = 0; i < iters; i++) ret = cipher.Decrypt(ret, subKey);
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(ret.GetSize(), range.GetMax());
        return start < end ? CollectionToArray<uint8_t>(SubSequence<uint8_t>(ret, Interval<size_t>(start, end))) : Array<uint8_t>();
    }
}