#include "MTE.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    MTE::MTE(const Cipher& cipher, const OneWayCipher& hash) : cipher(cipher), hash(hash) {}
    Array<uint8_t> MTE::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (!key.CheckChildrenSize(2)) return Array<uint8_t>();
        Array<uint8_t> ret = CollectionToArray<uint8_t>(data);
        return ret.AddSequence(hash.Encrypt(ret, key.children.AtUnsafe(1))) ? cipher.Encrypt(ret, key.children.AtUnsafe(0)) : Array<uint8_t>();
    }
    Array<uint8_t> MTE::DecryptPartial(const Sequence<uint8_t>& data_, const CipherKey& key, const Interval<size_t>& range) const {
        if (!key.CheckChildrenSize(3) || !key.children.AtUnsafe(2).CheckDataSize(sizeof(size_t))) return Array<uint8_t>();
        const size_t min = range.GetMin();
        const size_t max = range.GetMax();
        if (min >= max) return Array<uint8_t>();
        const Array<uint8_t> data = cipher.Decrypt(data_, key.children.AtUnsafe(0));
        const size_t size = data.GetSize();
        const size_t macSize = key.children.AtUnsafe(2).data.AsT<size_t>().Get();
        const SubSequence<uint8_t> ret = SubSequence<uint8_t>(data, 0, size - macSize);
        if (!hash.IsEncrypted<uint8_t>(ret, SubSequence<uint8_t>(data, size - macSize, size), key.children.AtUnsafe(1))) return Array<uint8_t>();
        const size_t end = Min<size_t>(max, size);
        return min < end ? CollectionToArray<uint8_t>(SubSequence<uint8_t>(ret, Interval<size_t>(min, end))) : Array<uint8_t>();
    }
}