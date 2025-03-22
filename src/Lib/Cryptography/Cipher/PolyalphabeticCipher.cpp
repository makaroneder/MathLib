#include "PolyalphabeticCipher.hpp"
#include "../../Interfaces/WrapSequence.hpp"

namespace MathLib {
    Array<uint8_t> PolyalphabeticCipher::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, bool encrypt) const {
        EmptyBenchmark
        const size_t size = data.GetSize();
        const MathLib::WrapSequence<uint64_t> wrap = MathLib::WrapSequence<uint64_t>(key, size);
        MathLib::Array<uint8_t> ret = MathLib::Array<uint8_t>(size);
        if (encrypt) for (size_t i = 0; i < size; i++) ret.At(i) = data.At(i) + wrap.At(i);
        else for (size_t i = 0; i < size; i++) ret.At(i) = data.At(i) - wrap.At(i);
        ReturnFromBenchmark(ret);
    }
}