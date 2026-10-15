#include "ETM.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    ETM::ETM(const Cipher& cipher, const OneWayCipher& hash) : cipher(cipher), hash(hash) {}
    Array<uint8_t> ETM::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (!key.CheckChildrenSize(2)) return Array<uint8_t>();
        Array<uint8_t> ret = cipher.Encrypt(data, key.children.AtUnsafe(0));
        return ret.AddSequence(hash.Encrypt(ret, key.children.AtUnsafe(1))) ? ret : Array<uint8_t>();
    }
    Array<uint8_t> ETM::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        if (!key.CheckChildrenSize(3) || !key.children.AtUnsafe(2).CheckDataSize(sizeof(size_t))) return Array<uint8_t>();
        const size_t size = data.GetSize();
        const size_t macSize = key.children.AtUnsafe(2).data.AsT<size_t>().Get();
        const SubSequence<uint8_t> encrypted = SubSequence<uint8_t>(data, 0, size - macSize);
        if (!hash.IsEncrypted<uint8_t>(encrypted, SubSequence<uint8_t>(data, size - macSize, size), key.children.AtUnsafe(1))) return Array<uint8_t>();
        return cipher.DecryptPartial(encrypted, key.children.AtUnsafe(0), range);
    }
}