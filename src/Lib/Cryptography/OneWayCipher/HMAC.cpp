#include "HMAC.hpp"
#include "../../FunctionT.hpp"
#include "../../Interfaces/Sequence/SequenceUnion.hpp"
#include "../../Interfaces/Sequence/ExtendedSequence.hpp"
#include "../../Interfaces/Sequence/TransformSequence.hpp"

namespace MathLib {
    HMAC::HMAC(OneWayCipher& hash) : hash(hash) {}
    Array<uint8_t> HMAC::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (key.type != CipherKey::Type::MultiKey || key.children.GetSize() != 3) return Array<uint8_t>();
        const CipherKey key1 = key.children.AtUnsafe(0);
        if (key1.type != CipherKey::Type::Normal || key1.data.GetSize() != sizeof(BlockSize)) return Array<uint8_t>();
        const BlockSize blockSize = key1.data.AsT<BlockSize>().Get();
        const CipherKey key2 = key.children.AtUnsafe(1);
        if (key2.type != CipherKey::Type::Normal) return Array<uint8_t>();
        const CipherKey key3 = key.children.AtUnsafe(2);
        const Array<uint8_t> hashedKey = key2.data.GetSize() <= (size_t)blockSize ? key2.data.GetArray() : hash.Encrypt(key2.data, key3);
        const ExtendedSequence<uint8_t> blockSizedKey = ExtendedSequence<uint8_t>(hashedKey, 0, (size_t)blockSize - hashedKey.GetSize());
        const Function<uint8_t, uint8_t>& innerPaddingFunction = MakeFunctionT<uint8_t, uint8_t>([](uint8_t x) -> uint8_t {
            return x ^ innerPaddingValue;
        });
        const TransformSequence<uint8_t, uint8_t> innerPadding = TransformSequence<uint8_t, uint8_t>(blockSizedKey, innerPaddingFunction);
        const SequenceUnion<uint8_t> seqUnion1 = SequenceUnion<uint8_t>(innerPadding, data);
        const Function<uint8_t, uint8_t>& outerPaddingFunction = MakeFunctionT<uint8_t, uint8_t>([](uint8_t x) -> uint8_t {
            return x ^ outerPaddingValue;
        });
        const TransformSequence<uint8_t, uint8_t> outerPadding = TransformSequence<uint8_t, uint8_t>(blockSizedKey, outerPaddingFunction);
        const Array<uint8_t> arr = hash.Encrypt(seqUnion1, key3);
        const SequenceUnion<uint8_t> seqUnion2 = SequenceUnion<uint8_t>(outerPadding, arr);
        return hash.Encrypt(seqUnion2, key3);
    }
}