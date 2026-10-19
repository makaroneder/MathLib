#include "DSA.hpp"
#include "../../BigInt/NaturalNumber.hpp"

namespace MathLib {
    Array<Array<uint8_t>> DSA::CreateSignature(const Sequence<uint8_t>& hash, const CipherKey& key) const {
        if (!key.CheckDataSize(4)) return Array<Array<uint8_t>>();
        const NaturalNumber p = key.children.AtUnsafe(0).data;
        const NaturalNumber q = key.children.AtUnsafe(1).data;
        const NaturalNumber g = key.children.AtUnsafe(2).data;
        const NaturalNumber privateKey = key.children.AtUnsafe(3).data;
        const Expected<NaturalNumber> q2 = q - NaturalNumber::FromT<uint8_t>(2);
        if (!q2.HasValue()) return Array<Array<uint8_t>>();
        const NaturalNumber k = q2.Get().RandomLessThanEqual() + NaturalNumber::FromT<uint8_t>(1);
        const NaturalNumber r = g.ModularExponentiation(k, p) % q;
        if (r.IsZero()) return CreateSignature(hash, key);
        const NaturalNumber s = k.ModularExponentiation(q2.Get(), q) * ((NaturalNumber(hash) + privateKey * r) % q) % q;
        if (s.IsZero()) return CreateSignature(hash, key);
        return MakeArray<Array<uint8_t>>(r.data, s.data);
    }
    bool DSA::VerifySignature(const Sequence<Array<uint8_t>>& signature, const Sequence<uint8_t>& hash, const CipherKey& key) const {
        if (!key.CheckDataSize(4) || signature.GetSize() != 2) return false;
        const NaturalNumber p = key.children.AtUnsafe(0).data;
        const NaturalNumber q = key.children.AtUnsafe(1).data;
        const NaturalNumber g = key.children.AtUnsafe(2).data;
        const NaturalNumber publicKey = key.children.AtUnsafe(3).data;
        const NaturalNumber r = signature.AtUnsafe(0);
        if (r.IsZero() || q.LessThanEqual(r)) return false;
        const NaturalNumber s = signature.AtUnsafe(1);
        if (s.IsZero() || q.LessThanEqual(s)) return false;
        const NaturalNumber w = s.GetModularInverseByFermatsLittleTheorem(q);
        return r == g.ModularExponentiation(NaturalNumber(hash) % q * w % q, p) * publicKey.ModularExponentiation(r % q * w % q, p) % p % q;
    }
}