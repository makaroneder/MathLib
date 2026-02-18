#include "NestedCipher.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    NestedCipher::NestedCipher(const Sequence<Cipher*>& ciphers) : ciphers(CollectionToArray<Cipher*>(ciphers)) {}
    NestedCipher::~NestedCipher(void) {
        for (Cipher*& cipher : ciphers) delete cipher;
    }
    Array<uint8_t> NestedCipher::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        const size_t size = ciphers.GetSize();
        if (key.type != CipherKey::Type::MultiKey || key.children.GetSize() != size) return Array<uint8_t>();
        Array<uint8_t> ret = CollectionToArray<uint8_t>(data);
        for (size_t i = 0; i < size; i++) ret = ciphers.AtUnsafe(i)->Encrypt(ret, key.children.AtUnsafe(i));
        return ret;
    }
    Array<uint8_t> NestedCipher::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        const size_t size = ciphers.GetSize();
        if (key.type != CipherKey::Type::MultiKey || key.children.GetSize() != size) return Array<uint8_t>();
        Array<uint8_t> ret = CollectionToArray<uint8_t>(data);
        for (size_t i = size; i; i--) ret = ciphers.AtUnsafe(i - 1)->Decrypt(ret, key.children.AtUnsafe(i - 1));
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(ret.GetSize(), range.GetMax());
        return start < end ? CollectionToArray<uint8_t>(SubSequence<uint8_t>(ret, Interval<size_t>(start, end))) : Array<uint8_t>();
    }
}