#include "BytePairEncodingGrammar.hpp"
#include "../../../String.hpp"

namespace MathLib {
    BytePairEncodingGrammar::BytePairEncodingGrammar(void) : substitutes(), string() {
        EmptyBenchmark
    }
    BytePairEncodingGrammar::BytePairEncodingGrammar(const Sequence<uint8_t>& data) : substitutes(), string(data.GetSize()) {
        StartBenchmark
        const size_t size = string.GetSize();
        for (size_t i = 0; i < size; i++) string.At(i) = data.At(i);
        EndBenchmark
    }
    bool BytePairEncodingGrammar::Save(Writable& file) const {
        StartBenchmark
        size_t size = substitutes.GetSize();
        if (!file.Write<size_t>(size)) ReturnFromBenchmark(false);
        for (size_t i = 0; i < size; i++)
            if (!substitutes.At(i).Save(file)) ReturnFromBenchmark(false);
        size = string.GetSize();
        if (!file.Write<size_t>(size)) ReturnFromBenchmark(false);
        for (size_t i = 0; i < size; i++)
            if (!file.Write<size_t>(string.At(i))) ReturnFromBenchmark(false);
        ReturnFromBenchmark(true);
    }
    bool BytePairEncodingGrammar::Load(Readable& file) {
        StartBenchmark
        size_t size;
        if (!file.Read<size_t>(size)) ReturnFromBenchmark(false);
        substitutes = Array<BytePairEncodingSubstitute>(size);
        for (size_t i = 0; i < size; i++)
            if (!substitutes.At(i).Load(file)) ReturnFromBenchmark(false);
        if (!file.Read<size_t>(size)) ReturnFromBenchmark(false);
        string = Array<size_t>(size);
        for (size_t i = 0; i < size; i++)
            if (!file.Read<size_t>(string.At(i))) ReturnFromBenchmark(false);
        ReturnFromBenchmark(true);
    }
    String BytePairEncodingGrammar::ToString(const Sequence<char>& padding) const {
        StartBenchmark
        String ret = "{\n";
        const String padd = '\t'_M + padding;
        for (const BytePairEncodingSubstitute& substitute : substitutes) ret += substitute.ToString(padd) + '\n';
        ret += padd + "String = (";
        const size_t size = string.GetSize();
        for (size_t i = 0; i < size; i++) {
            if (i) ret += ", ";
            ret += MathLib::ToString(string.At(i), 10);
        }
        ReturnFromBenchmark(ret + ")\n" + padding + '}');
    }
}