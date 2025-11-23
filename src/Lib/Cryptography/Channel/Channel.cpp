#include "Channel.hpp"
#include "../../FunctionT.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"
#include "../../Interfaces/Sequence/TransformSequence.hpp"

namespace MathLib {
    Channel::Channel(OneWayCipher* channel, NestedCipherData* cipher) : channel(channel), cipher(cipher) {}
    Channel::~Channel(void) {
        delete channel;
        delete cipher;
    }
    Array<uint8_t> Channel::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        const SubSequence<uint64_t> cipherKey = SubSequence<uint64_t>(key, Interval<size_t>(0, cipher->GetKeySize()));
        return cipher->Decrypt(channel->Encrypt(cipher->Encrypt(data, cipherKey), SubSequence<uint64_t>(key, Interval<size_t>(cipher->GetKeySize(), key.GetSize()))), cipherKey);
    }
    String Channel::SendString(const Sequence<char>& data, const Sequence<uint64_t>& key) const {
        return CollectionToString(TransformSequence<char, uint8_t>(EncryptT<char>(data, key), MakeFunctionT<char, uint8_t>([](uint8_t ret) -> char {
            return ret;
        })));
    }
}