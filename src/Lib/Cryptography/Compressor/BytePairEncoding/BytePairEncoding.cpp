#include "BytePairEncoding.hpp"
#include "../../../FunctionT.hpp"
#include "../../../SingleTypePair.hpp"
#include "../../../Interfaces/ByteArray.hpp"
#include "../../../Interfaces/Dictionary.hpp"

namespace MathLib {
    BytePairEncodingGrammar BytePairEncoding::GenerateGrammar(const Sequence<uint8_t>& input) const {
        StartBenchmark
        size_t uniqueName = UINT8_MAX + 1;
        const Function<size_t, size_t>& successorFunction = MakeFunctionT<size_t, size_t>(nullptr, [](const void*, size_t count) -> size_t {
            return count + 1;
        });
        Dictionary<SingleTypePair<size_t>, size_t> dictionary;
        Array<size_t> buff;
        size_t size = input.GetSize();
        BytePairEncodingGrammar ret = BytePairEncodingGrammar(input);
        while (true) {
            for (size_t i = 0; i < size - 1; i++)
                if (!dictionary.AddOrModify(SingleTypePair<size_t>(ret.string.At(i), ret.string.At(i + 1)), successorFunction, 1)) ReturnFromBenchmark(BytePairEncodingGrammar());
            DictionaryElement<SingleTypePair<size_t>, size_t> tmp;
            for (DictionaryElement<SingleTypePair<size_t>, size_t>& x : dictionary)
                if (x.value > tmp.value) tmp = x;
            if (tmp.value <= 1) break;
            const SingleTypePair<size_t> pair = tmp.GetKey();
            if (!ret.substitutes.Add(BytePairEncodingSubstitute(uniqueName, pair.first, pair.second))) ReturnFromBenchmark(BytePairEncodingGrammar());
            for (size_t j = 0; j < size; j++) {
                if (j + 1 != size && ret.string.At(j) == pair.first && ret.string.At(j + 1) == pair.second) {
                    if (!buff.Add(uniqueName)) ReturnFromBenchmark(BytePairEncodingGrammar());
                    j++;
                }
                else if (!buff.Add(ret.string.At(j))) ReturnFromBenchmark(BytePairEncodingGrammar());
            }
            uniqueName++;
            ret.string = buff;
            size = ret.string.GetSize();
            if (!dictionary.Reset() || !buff.Reset()) ReturnFromBenchmark(BytePairEncodingGrammar());
        }
        ReturnFromBenchmark(ret);
    }
    Array<uint8_t> BytePairEncoding::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        StartBenchmark
        if (!key.IsEmpty()) ReturnFromBenchmark(Array<uint8_t>());
        ByteArray device = ByteArray();
        ReturnFromBenchmark(GenerateGrammar(data).Save(device) ? device.GetArray() : Array<uint8_t>());
    }
    Array<uint8_t> BytePairEncoding::DecryptSingle(size_t name, const Sequence<BytePairEncodingSubstitute>& substitutes) const {
        StartBenchmark
        if (name <= UINT8_MAX) ReturnFromBenchmark(MakeArray<uint8_t>(name));
        const size_t size = substitutes.GetSize();
        for (size_t i = 0; i < size; i++) {
            const BytePairEncodingSubstitute substitute = substitutes.At(i);
            if (substitute.name == name) {
                Array<uint8_t> ret = DecryptSingle(substitute.a, substitutes);
                ret += DecryptSingle(substitute.b, substitutes);
                ReturnFromBenchmark(ret);
            }
        }
        ReturnFromBenchmark(Array<uint8_t>());
    }
    Array<uint8_t> BytePairEncoding::DecryptReadablePartial(Readable& readable, const Sequence<uint64_t>& key, const Interval<size_t>& range) const {
        StartBenchmark
        if (!key.IsEmpty()) ReturnFromBenchmark(Array<uint8_t>());
        size_t size = 0;
        if (!readable.Read<size_t>(size)) ReturnFromBenchmark(Array<uint8_t>());
        Array<BytePairEncodingSubstitute> substitutes = Array<BytePairEncodingSubstitute>(size);
        for (size_t i = 0; i < size; i++)
            if (!substitutes.At(i).Load(readable)) ReturnFromBenchmark(Array<uint8_t>());
        if (!readable.Read<size_t>(size)) ReturnFromBenchmark(Array<uint8_t>());
        size_t pos = 0;
        Array<uint8_t> ret;
        for (size_t i = 0; i < size; i++) {
            size_t tmp;
            if (!readable.Read<size_t>(tmp)) ReturnFromBenchmark(Array<uint8_t>());
            const Array<uint8_t> add = DecryptSingle(tmp, substitutes);
            const size_t size = add.GetSize();
            bool end = false;
            for (size_t j = 0; j < size; j++) {
                if (range.Contains(pos++) && !ret.Add(add.At(j))) ReturnFromBenchmark(Array<uint8_t>());
                if (pos >= range.GetMax()) {
                    end = true;
                    break;
                }
            }
            if (end) break;
        }
        ReturnFromBenchmark(ret);
    }
}