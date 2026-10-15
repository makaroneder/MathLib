#include "EAM.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    EAM::EAM(const Cipher& cipher, const OneWayCipher& hash) : cipher(cipher), hash(hash) {}
    Array<uint8_t> EAM::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (!key.CheckChildrenSize(2)) return Array<uint8_t>();
        Array<uint8_t> ret = cipher.Encrypt(data, key.children.AtUnsafe(0));
        return ret.AddSequence(hash.Encrypt(data, key.children.AtUnsafe(1))) ? ret : Array<uint8_t>();
    }
    Array<uint8_t> EAM::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        if (!key.CheckChildrenSize(3) || !key.children.AtUnsafe(2).CheckDataSize(sizeof(size_t))) return Array<uint8_t>();
        const size_t min = range.GetMin();
        const size_t max = range.GetMax();
        if (min >= max) return Array<uint8_t>();
        const size_t size = data.GetSize();
        const size_t macSize = key.children.AtUnsafe(2).data.AsT<size_t>().Get();
        const Array<uint8_t> ret = cipher.Decrypt(SubSequence<uint8_t>(data, 0, size - macSize), key.children.AtUnsafe(0));
        if (!hash.IsEncrypted<uint8_t>(ret, SubSequence<uint8_t>(data, size - macSize, size), key.children.AtUnsafe(1))) return Array<uint8_t>();
        const size_t end = Min<size_t>(max, size);
        return min < end ? CollectionToArray<uint8_t>(SubSequence<uint8_t>(ret, Interval<size_t>(min, end))) : Array<uint8_t>();
    }
}