#include <MathLib.hpp>
#include <iostream>

MathLib::ChemicalMolecule ParseMolecule(const MathLib::String& str, size_t& i) {
    const size_t size = str.GetSize();
    MathLib::Array<MathLib::ChemicalElement> elements;
    MathLib::SkipWhiteSpace(str, i);
    while (i < size && MathLib::IsUpper(str.At(i))) {
        MathLib::String name = str.At(i++);
        while (i < size && MathLib::IsLower(str.At(i))) name += str.At(i++);
        MathLib::String count;
        while (i < size && MathLib::IsDigit(str.At(i))) count += str.At(i++);
        elements.Add(MathLib::ChemicalElement(name, count.IsEmpty() ? 1 : MathLib::StringToNumber(count), false));
    }
    return MathLib::ChemicalMolecule(elements, 1);
}
MathLib::Array<MathLib::ChemicalReaction> ParseReactions(const MathLib::String& str) {
    MathLib::Array<MathLib::ChemicalReaction> ret;
    const MathLib::Array<MathLib::String> split = MathLib::Split(str, '\n', false);
    for (const MathLib::String& str : split) {
        const size_t size = str.GetSize();
        MathLib::Array<MathLib::ChemicalMolecule> left;
        size_t i = 0;
        while (i < size) {
            if (!left.Add(ParseMolecule(str, i))) return MathLib::Array<MathLib::ChemicalReaction>();
            MathLib::SkipWhiteSpace(str, i);
            if (i < size && str.At(i) == '+') {
                i++;
                MathLib::SkipWhiteSpace(str, i);
            }
            else if (i + 1 < size && str.At(i) == '=' && str.At(i + 1) == '>') {
                i += 2;
                MathLib::SkipWhiteSpace(str, i);
                break;
            }
        }
        MathLib::Array<MathLib::ChemicalMolecule> right;
        while (i < size) {
            if (!right.Add(ParseMolecule(str, i))) return MathLib::Array<MathLib::ChemicalReaction>();
            MathLib::SkipWhiteSpace(str, i);
            if (i < size && str.At(i) == '+') {
                i++;
                MathLib::SkipWhiteSpace(str, i);
            }
        }
        if (!ret.Add(MathLib::ChemicalReaction(left, right))) return MathLib::Array<MathLib::ChemicalReaction>();
    }
    return ret;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2) MathLib::Panic(MathLib::String("Usage: ") + argv[0] + " <input file>");
        const MathLib::Array<MathLib::ChemicalReaction> reactions = ParseReactions(MathLib::HostFileSystem().Open(argv[1], MathLib::OpenMode::Read).ReadUntil('\0'));
        for (size_t i = 0; i < reactions.GetSize(); i++)
            std::cout << reactions.At(i) << '\n' << reactions.At(i).Balance().Get("Failed to balance chemical reaction") << (i + 1 == reactions.GetSize() ? "" : "\n") << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}