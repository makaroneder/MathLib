#include "RSA.hpp"
#include "../../BigInt/NaturalNumber.hpp"

namespace MathLib {
    Array<uint8_t> RSA::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (!key.CheckChildrenSize(2)) return Array<uint8_t>();
        const NaturalNumber mod = key.children.AtUnsafe(0).data;
        const NaturalNumber nat = data;
        if (mod.LessThanEqual(nat)) return Array<uint8_t>();
        return nat.ModularExponentiation(key.children.AtUnsafe(1).data, mod).data;
    }
    Array<uint8_t> RSA::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        if (!key.CheckChildrenSize(2)) return Array<uint8_t>();
        const NaturalNumber nat = data;
        const Array<uint8_t> ret = nat.ModularExponentiation(key.children.AtUnsafe(1).data, key.children.AtUnsafe(0).data).data;
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(ret.GetSize(), range.GetMax());
        return start < end ? Array<uint8_t>(ret.GetValue() + start, end - start) : Array<uint8_t>();
    }
}