#include "FeistelCipher.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"
#include "../../Interfaces/Sequence/SequenceUnion.hpp"

namespace MathLib {
    FeistelCipher::FeistelCipher(OneWayCipher& roundFunction) : roundFunction(roundFunction) {}
    Array<uint8_t> FeistelCipher::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (key.type != CipherKey::Type::MultiKey) return Array<uint8_t>();
        const size_t keySize = key.children.GetSize();
        const size_t size = data.GetSize();
        if (size % 2) return Array<uint8_t>();
        Array<uint8_t> left = size / 2;
        for (size_t i = 0; i < size / 2; i++) left.AtUnsafe(i) = data.AtUnsafe(i);
        Array<uint8_t> right = size / 2;
        for (size_t i = 0; i < size / 2; i++) right.AtUnsafe(i) = data.AtUnsafe(i + size / 2);
        for (size_t i = 0; i < keySize; i++) {
            Array<uint8_t> tmp = roundFunction.Encrypt(right, key.children.AtUnsafe(i));
            if (tmp.GetSize() != size / 2) return Array<uint8_t>();
            for (size_t j = 0; j < tmp.GetSize(); j++) tmp.AtUnsafe(j) ^= left.AtUnsafe(j);
            left = right;
            right = tmp;
        }
        return right.AddSequence(left) ? right : Array<uint8_t>();
    }
    Array<uint8_t> FeistelCipher::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        if (key.type != CipherKey::Type::MultiKey) return Array<uint8_t>();
        const size_t keySize = key.children.GetSize();
        const size_t size = data.GetSize();
        if (size % 2) return Array<uint8_t>();
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(size, range.GetMax());
        if (start >= end) return Array<uint8_t>();
        Array<uint8_t> right = size / 2;
        for (size_t i = 0; i < size / 2; i++) right.AtUnsafe(i) = data.AtUnsafe(i);
        Array<uint8_t> left = size / 2;
        for (size_t i = 0; i < size / 2; i++) left.AtUnsafe(i) = data.AtUnsafe(i + size / 2);
        for (size_t i = keySize; i; i--) {
            Array<uint8_t> tmp = roundFunction.Encrypt(left, key.children.AtUnsafe(i - 1));
            if (tmp.GetSize() != size / 2) return Array<uint8_t>();
            for (size_t j = 0; j < tmp.GetSize(); j++) tmp.AtUnsafe(j) ^= right.AtUnsafe(j);
            right = left;
            left = tmp;
        }
        const SequenceUnion<uint8_t> seq = SequenceUnion<uint8_t>(left, right);
        const SubSequence<uint8_t> sub = SubSequence<uint8_t>(seq, Interval<size_t>(start, end));
        return CollectionToArray<uint8_t>(sub);
    }
}