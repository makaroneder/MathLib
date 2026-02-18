#include "BytePairEncoding.hpp"
#include "../../../FunctionT.hpp"
#include "../../../SingleTypePair.hpp"
#include "../../../Interfaces/Dictionary.hpp"
#include "../../../Interfaces/Sequence/ByteArray.hpp"

namespace MathLib {
    BytePairEncodingGrammar BytePairEncoding::GenerateGrammar(const Sequence<uint8_t>& input) {
        size_t uniqueName = UINT8_MAX + 1;
        const Function<size_t, size_t>& successorFunction = MakeFunctionT<size_t, size_t>([](size_t count) -> size_t {
            return count + 1;
        });
        Dictionary<SingleTypePair<size_t>, size_t> dictionary;
        Array<size_t> buff;
        size_t size = input.GetSize();
        BytePairEncodingGrammar ret = BytePairEncodingGrammar(input);
        while (true) {
            for (size_t i = 0; i < size - 1; i++)
                if (!dictionary.AddOrModify(SingleTypePair<size_t>(ret.string.AtUnsafe(i), ret.string.AtUnsafe(i + 1)), successorFunction, 1)) return BytePairEncodingGrammar();
            DictionaryElement<SingleTypePair<size_t>, size_t> tmp;
            for (DictionaryElement<SingleTypePair<size_t>, size_t>& x : dictionary)
                if (x.value > tmp.value) tmp = x;
            if (tmp.value <= 1) break;
            const SingleTypePair<size_t> pair = tmp.GetKey();
            if (!ret.substitutes.Add(BytePairEncodingSubstitute(uniqueName, pair.first, pair.second))) return BytePairEncodingGrammar();
            for (size_t j = 0; j < size; j++) {
                if (j + 1 != size && ret.string.AtUnsafe(j) == pair.first && ret.string.AtUnsafe(j + 1) == pair.second) {
                    if (!buff.Add(uniqueName)) return BytePairEncodingGrammar();
                    j++;
                }
                else if (!buff.Add(ret.string.AtUnsafe(j))) return BytePairEncodingGrammar();
            }
            uniqueName++;
            ret.string = buff;
            size = ret.string.GetSize();
            if (!dictionary.Reset() || !buff.Reset()) return BytePairEncodingGrammar();
        }
        return ret;
    }
    Array<uint8_t> BytePairEncoding::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (!key.IsEmpty()) return Array<uint8_t>();
        ByteArray device = ByteArray();
        return GenerateGrammar(data).Save(device) ? device.GetArray() : Array<uint8_t>();
    }
    Array<uint8_t> BytePairEncoding::DecryptSingle(size_t name, const Sequence<BytePairEncodingSubstitute>& substitutes) const {
        if (name <= UINT8_MAX) return MakeArray<uint8_t>(name);
        const size_t size = substitutes.GetSize();
        for (size_t i = 0; i < size; i++) {
            const BytePairEncodingSubstitute substitute = substitutes.AtUnsafe(i);
            if (substitute.name == name) {
                Array<uint8_t> ret = DecryptSingle(substitute.a, substitutes);
                ret += DecryptSingle(substitute.b, substitutes);
                return ret;
            }
        }
        return Array<uint8_t>();
    }
    Array<uint8_t> BytePairEncoding::DecryptReadablePartial(Readable& readable, const CipherKey& key, const Interval<size_t>& range) const {
        if (!key.IsEmpty()) return Array<uint8_t>();
        size_t size = 0;
        if (!readable.Read<size_t>(size)) return Array<uint8_t>();
        Array<BytePairEncodingSubstitute> substitutes = Array<BytePairEncodingSubstitute>(size);
        for (size_t i = 0; i < size; i++)
            if (!substitutes.At(i).Load(readable)) return Array<uint8_t>();
        if (!readable.Read<size_t>(size)) return Array<uint8_t>();
        size_t pos = 0;
        Array<uint8_t> ret;
        for (size_t i = 0; i < size; i++) {
            size_t tmp;
            if (!readable.Read<size_t>(tmp)) return Array<uint8_t>();
            const Array<uint8_t> add = DecryptSingle(tmp, substitutes);
            const size_t size = add.GetSize();
            bool end = false;
            for (size_t j = 0; j < size; j++) {
                if (range.Contains(pos++) && !ret.Add(add.AtUnsafe(j))) return Array<uint8_t>();
                if (pos >= range.GetMax()) {
                    end = true;
                    break;
                }
            }
            if (end) break;
        }
        return ret;
    }
}