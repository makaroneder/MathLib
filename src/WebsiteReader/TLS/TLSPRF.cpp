#include "TLSPRF.hpp"
#include <Interfaces/Sequence/SubSequence.hpp>

TLSPRF::TLSPRF(const MathLib::HMAC& hmac, MathLib::HMAC::BlockSize blockSize, const MathLib::CipherKey& hashKey) : hashKey(hashKey), hmac(hmac), blockSize(blockSize) {}
MathLib::Array<uint8_t> TLSPRF::Encrypt(const MathLib::Sequence<uint8_t>& data, const MathLib::CipherKey& key) const {
    if (!key.CheckChildrenSize(3)) return MathLib::Array<uint8_t>();
    if (!key.children.AtUnsafe(0).CheckDataSize(sizeof(size_t))) return MathLib::Array<uint8_t>();
    const MathLib::CipherKey hmacKey = MathLib::CipherKey(MathLib::MakeArray<MathLib::CipherKey>(
        MathLib::CipherKey(MathLib::ByteArray::ToByteArray<MathLib::HMAC::BlockSize>(blockSize)),
        MathLib::CipherKey(key.children.AtUnsafe(1).data),
        hashKey
    ));
    const size_t size = key.children.AtUnsafe(0).data.AsT<size_t>().Get();
    MathLib::Array<uint8_t> label = MathLib::CollectionToArray<uint8_t>(data);
    if (!label.AddSequence(key.children.AtUnsafe(2).data)) return MathLib::Array<uint8_t>();
    MathLib::Array<uint8_t> prev = label;
    MathLib::Array<uint8_t> ret = prev;
    while (ret.GetSize() < size) {
        if (!prev.AddSequence(label)) return MathLib::Array<uint8_t>();
        prev = hmac.Encrypt(prev, hmacKey);
        if (!ret.AddSequence(prev)) return MathLib::Array<uint8_t>();
    }
    if (ret.GetSize() == size) return ret;
    return MathLib::CollectionToArray<uint8_t>(MathLib::SubSequence<uint8_t>(ret, MathLib::Interval<size_t>(0, size)));
}