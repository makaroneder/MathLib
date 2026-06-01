#include "EDE.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    EDE::EDE(Cipher& cipher) : cipher(cipher) {}
    Array<uint8_t> EDE::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        return key.type == CipherKey::Type::MultiKey && key.children.GetSize() == 3 ? cipher.Encrypt(cipher.Decrypt(cipher.Encrypt(data, key.children.AtUnsafe(0)), key.children.AtUnsafe(1)), key.children.AtUnsafe(2)) : Array<uint8_t>();
    }
    Array<uint8_t> EDE::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        return key.type == CipherKey::Type::MultiKey && key.children.GetSize() == 3 ? cipher.DecryptPartial(cipher.Encrypt(cipher.Decrypt(data, key.children.AtUnsafe(2)), key.children.AtUnsafe(1)), key.children.AtUnsafe(0), range) : Array<uint8_t>();
    }
}