#include <MathLib.hpp>
#include <iostream>

ChemicalMolecule ParseMolecule(const String& str, size_t& i) {
    const size_t size = str.GetSize();
    Array<ChemicalElement> elements;
    SkipWhiteSpace(str, i);
    while (i < size && IsUpper(str.At(i))) {
        String name = str.At(i++);
        while (i < size && IsLower(str.At(i))) name += str.At(i++);
        String count;
        while (i < size && IsDigit(str.At(i))) count += str.At(i++);
        elements.Add(ChemicalElement(name, count.IsEmpty() ? 1 : StringToNumber(count), false));
    }
    return ChemicalMolecule(elements, 1);
}
Array<ChemicalReaction> ParseReactions(const String& str) {
    Array<ChemicalReaction> ret;
    const Array<String> split = Split(str, '\n', false);
    for (const String& str : split) {
        const size_t size = str.GetSize();
        Array<ChemicalMolecule> left;
        size_t i = 0;
        while (i < size) {
            if (!left.Add(ParseMolecule(str, i))) return Array<ChemicalReaction>();
            SkipWhiteSpace(str, i);
            if (i < size && str.At(i) == '+') {
                i++;
                SkipWhiteSpace(str, i);
            }
            else if (i + 1 < size && str.At(i) == '=' && str.At(i + 1) == '>') {
                i += 2;
                SkipWhiteSpace(str, i);
                break;
            }
        }
        Array<ChemicalMolecule> right;
        while (i < size) {
            if (!right.Add(ParseMolecule(str, i))) return Array<ChemicalReaction>();
            SkipWhiteSpace(str, i);
            if (i < size && str.At(i) == '+') {
                i++;
                SkipWhiteSpace(str, i);
            }
        }
        if (!ret.Add(ChemicalReaction(left, right))) return Array<ChemicalReaction>();
    }
    return ret;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        #ifdef Debug
        const Test test = TestSelf();
        const size_t tests = test.GetRecordCount();
        const size_t passed = test.GetPassed();
        std::cout << test << passed << "/" << tests << " tests passed" << std::endl;
        if (passed != tests) Panic("Some tests failed");
        #endif
        if (argc < 2) Panic(String("Usage: ") + argv[0] + " <input file>");
        const Array<ChemicalReaction> reactions = ParseReactions(HostFileSystem().Open(argv[1], OpenMode::Read).ReadUntil('\0'));
        for (size_t i = 0; i < reactions.GetSize(); i++)
            std::cout << reactions.At(i) << '\n' << reactions.At(i).Balance().Get("Failed to balance chemical reaction") << (i + 1 == reactions.GetSize() ? "" : "\n") << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}