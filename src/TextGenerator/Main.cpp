#include <Interfaces/Sequence/TransformSequence.hpp>
#include <Interfaces/Sequence/SubSequence.hpp>
#include <FunctionT.hpp>
#include <Host.hpp>
#include <iostream>

MathLib::Array<MathLib::String> SplitIntoWords(const MathLib::String& str) {
    const size_t size = str.GetSize();
    MathLib::Array<MathLib::String> ret;
    MathLib::String current;
    for (size_t i = 0; i < size; i++) {
        const char chr = str.AtUnsafe(i);
        if (!MathLib::IsWhiteSpace(chr)) current += chr;
        else if (!ret.Add(current) || !current.Reset()) return MathLib::Array<MathLib::String>();
        if (i + 1 == size && !ret.Add(current)) return MathLib::Array<MathLib::String>();
    }
    return ret;
}
MathLib::Array<size_t> GetNextWords(const MathLib::Sequence<MathLib::String>& text, const MathLib::Sequence<size_t>& index) {
    const size_t size = text.GetSize();
    const size_t wordCount = index.GetSize();
    const MathLib::TransformSequence<MathLib::String, size_t> words = MathLib::TransformSequence<MathLib::String, size_t>(index, text);
    MathLib::Array<size_t> ret;
    size_t start = 0;
    while (true) {
        start = text.Find(words, start);
        if (start == SIZE_MAX) break;
        start += wordCount;
        if (start >= size) break;
        if (!ret.Add(start)) return MathLib::Array<size_t>();
    }
    return ret;
}
size_t Random(size_t min, size_t max) {
    return MathLib::Round(MathLib::RandomNumber<MathLib::num_t>(min, max));
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        #ifndef Debug
        srand(time(nullptr));
        #endif
        const MathLib::String data = "0 0 0 1 0 1 1 1 1 0 0 0 0 1 0"_M;
        const MathLib::Array<MathLib::String> text = SplitIntoWords(data);
        const size_t maxSize = 10;
        const size_t order = 2;
        MathLib::Array<size_t> words = MathLib::MakeArray<size_t>(Random(0, text.GetSize() - 1));
        for (size_t i = 0; i < maxSize; i++) {
            const size_t wordCount = words.GetSize();
            if (i) std::cout << ' ';
            std::cout << text.AtUnsafe(words.AtUnsafe(wordCount - 1));
            const MathLib::Array<size_t> next = GetNextWords(text, MathLib::SubSequence<size_t>(words, MathLib::Interval<size_t>(wordCount - MathLib::Min<size_t>(wordCount, order), wordCount)));
            const size_t size = next.GetSize();
            if (!size) break;
            if (!words.Add(next.AtUnsafe(Random(0, size - 1)))) MathLib::Panic("Failed to add word to history");
        }
        std::cout << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}