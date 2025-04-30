#ifndef FileCipher_H
#define FileCipher_H
#include "FileCipherData.hpp"
#include <Cryptography/ReadableCipher.hpp>
#include <Interfaces/ByteDevice.hpp>

struct FileCipher : MathLib::ReadableCipher {
    [[nodiscard]] virtual bool IsValidHeader(MathLib::Readable& device, const MathLib::Sequence<uint64_t>& key) const = 0;
    [[nodiscard]] virtual bool IsValidFooter(MathLib::Readable& device, const MathLib::Sequence<uint64_t>& key) const = 0;
    [[nodiscard]] virtual bool Create(MathLib::Writable& device, const MathLib::Sequence<uint64_t>& key) const = 0;
    [[nodiscard]] virtual bool UpdateHeaderAndFooter(MathLib::ByteDevice& device, const MathLib::Sequence<uint64_t>& key) const = 0;
    [[nodiscard]] virtual FileCipherData Identify(MathLib::Readable& device, const MathLib::Sequence<uint64_t>& key) const = 0;
};

#endif