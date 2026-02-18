#include "Channel.hpp"
#include "../../FunctionT.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"
#include "../../Interfaces/Sequence/TransformSequence.hpp"

namespace MathLib {
    Channel::Channel(OneWayCipher& channel, Cipher& cipher) : channel(channel), cipher(cipher) {}
    Array<uint8_t> Channel::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (key.type != CipherKey::Type::MultiKey || key.children.GetSize() != 2) return Array<uint8_t>();
        return cipher.Decrypt(channel.Encrypt(cipher.Encrypt(data, key.children.AtUnsafe(1)), key.children.AtUnsafe(0)), key.children.AtUnsafe(1));
    }
    String Channel::SendString(const Sequence<char>& data, const CipherKey& key) const {
        return CollectionToString(TransformSequence<char, uint8_t>(EncryptT<char>(data, key), MakeFunctionT<char, uint8_t>([](uint8_t ret) -> char {
            return ret;
        })));
    }
}