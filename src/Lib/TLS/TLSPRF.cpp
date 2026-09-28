#include "TLSPRF.hpp"
#include "../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    TLSPRF::TLSPRF(const HMAC& hmac, HMAC::BlockSize blockSize, const CipherKey& hashKey) : hashKey(hashKey), hmac(hmac), blockSize(blockSize) {}
    Array<uint8_t> TLSPRF::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (!key.CheckChildrenSize(3)) return Array<uint8_t>();
        if (!key.children.AtUnsafe(0).CheckDataSize(sizeof(size_t))) return Array<uint8_t>();
        const CipherKey hmacKey = CipherKey(MakeArray<CipherKey>(
            CipherKey(ByteArray::ToByteArray<HMAC::BlockSize>(blockSize)),
            CipherKey(key.children.AtUnsafe(1).data),
            hashKey
        ));
        const size_t size = key.children.AtUnsafe(0).data.AsT<size_t>().Get();
        Array<uint8_t> label = CollectionToArray<uint8_t>(data);
        if (!label.AddSequence(key.children.AtUnsafe(2).data)) return Array<uint8_t>();
        Array<uint8_t> prev = label;
        Array<uint8_t> ret;
        while (ret.GetSize() < size) {
            prev = hmac.Encrypt(prev, hmacKey);
            Array<uint8_t> tmp = prev;
            if (!tmp.AddSequence(label) || !ret.AddSequence(hmac.Encrypt(tmp, hmacKey))) return Array<uint8_t>();
        }
        if (ret.GetSize() == size) return ret;
        return CollectionToArray<uint8_t>(SubSequence<uint8_t>(ret, Interval<size_t>(0, size)));
    }
}