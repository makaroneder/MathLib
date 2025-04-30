#include "Channel.hpp"
#include "../../FunctionT.hpp"
#include "../../Interfaces/SubSequence.hpp"
#include "../../Interfaces/TransformSequence.hpp"

namespace MathLib {
    Channel::Channel(MathLib::OneWayCipher* channel, MathLib::NestedCipherData* cipher) : channel(channel), cipher(cipher) {}
    Channel::~Channel(void) {
        delete channel;
        delete cipher;
    }
    MathLib::Array<uint8_t> Channel::Encrypt(const MathLib::Sequence<uint8_t>& data, const MathLib::Sequence<uint64_t>& key) const {
        const MathLib::SubSequence<uint64_t> cipherKey = MathLib::SubSequence<uint64_t>(key, MathLib::Interval<size_t>(0, cipher->GetKeySize()));
        return cipher->Decrypt(channel->Encrypt(cipher->Encrypt(data, cipherKey), MathLib::SubSequence<uint64_t>(key, MathLib::Interval<size_t>(cipher->GetKeySize(), key.GetSize()))), cipherKey);
    }
    MathLib::String Channel::SendString(const MathLib::Sequence<char>& data, const MathLib::Sequence<uint64_t>& key) const {
        return MathLib::CollectionToString(MathLib::TransformSequence<char, uint8_t>(EncryptT<char>(data, key), MathLib::MakeFunctionT<char, uint8_t>(nullptr, [](const void*, uint8_t ret) -> char {
            return ret;
        })));
    }
}