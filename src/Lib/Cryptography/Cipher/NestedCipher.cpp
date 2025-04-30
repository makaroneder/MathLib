#include "NestedCipher.hpp"
#include "../../Interfaces/SubSequence.hpp"

namespace MathLib {
    NestedCipher::NestedCipher(const Sequence<NestedCipherData*>& ciphers) : ciphers(CollectionToArray<NestedCipherData*>(ciphers)) {
        EmptyBenchmark
    }
    NestedCipher::~NestedCipher(void) {
        StartBenchmark
        for (NestedCipherData*& cipher : ciphers) delete cipher;
        EndBenchmark
    }
    Array<uint8_t> NestedCipher::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        StartBenchmark
        Array<uint8_t> ret = CollectionToArray<uint8_t>(data);
        size_t i = 0;
        for (const NestedCipherData* const& cipher : ciphers) {
            const size_t save = i;
            i += cipher->GetKeySize();
            ret = cipher->Encrypt(ret, SubSequence<uint64_t>(key, Interval<size_t>(save, i)));
        }
        ReturnFromBenchmark(ret);
    }
    Array<uint8_t> NestedCipher::DecryptPartial(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, const Interval<size_t>& range) const {
        StartBenchmark
        Array<uint8_t> ret = CollectionToArray<uint8_t>(data);
        const size_t size = ciphers.GetSize();
        size_t i = key.GetSize();
        for (size_t j = size; j; j--) {
            const size_t save = i;
            i -= ciphers.At(j - 1)->GetKeySize();
            ret = ciphers.At(j - 1)->Decrypt(ret, SubSequence<uint64_t>(key, Interval<size_t>(i, save)));
        }
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(ret.GetSize(), range.GetMax());
        ReturnFromBenchmark(start < end ? CollectionToArray<uint8_t>(SubSequence<uint8_t>(ret, Interval<size_t>(start, end))) : Array<uint8_t>());
    }
}