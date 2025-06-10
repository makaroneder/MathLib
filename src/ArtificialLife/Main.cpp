#include <Interfaces/TransformSequence.hpp>
#include <Interfaces/Comparable.hpp>
#include <FunctionT.hpp>
#include <String.hpp>
#include <Host.hpp>
#include <iostream>

struct Molecule : MathLib::Allocatable {
    [[nodiscard]] virtual size_t GetType(void) const = 0;
};
struct Reaction : MathLib::Comparable, MathLib::Printable {
    MathLib::Array<size_t> reactants;
    MathLib::Array<size_t> products;

    Reaction(void) : reactants(), products() {}
    Reaction(const MathLib::Sequence<size_t>& reactants, const MathLib::Sequence<size_t>& products) : reactants(MathLib::CollectionToArray<size_t>(reactants)), products(MathLib::CollectionToArray<size_t>(products)) {}
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override {
        MathLib::String ret = MathLib::CollectionToString(padding);
        const size_t size1 = reactants.GetSize();
        for (size_t i = 0; i < size1; i++) {
            if (i) ret += " + ";
            ret += MathLib::ToString(reactants.At(i), 10);
        }
        ret += " -> ";
        const size_t size2 = products.GetSize();
        for (size_t i = 0; i < size2; i++) {
            if (i) ret += " + ";
            ret += MathLib::ToString(products.At(i), 10);
        }
        return ret;
    }

    protected:
    [[nodiscard]] virtual bool Equals(const MathLib::Comparable& other_) const override {
        const Reaction& other = (const Reaction&)other_;
        return reactants == other.reactants && products == other.products;
    }
};
struct ArtificialChemistry {
    ArtificialChemistry(const MathLib::Sequence<Reaction>& reactions, const MathLib::Sequence<Molecule*>& molecules) : reactions(MathLib::CollectionToArray<Reaction>(reactions)), molecules(MathLib::CollectionToArray<Molecule*>(molecules)) {}
    ~ArtificialChemistry(void) {
        for (Molecule*& molecule : molecules) delete molecule;
    }

    protected:
    MathLib::Array<Reaction> reactions;
    MathLib::Array<Molecule*> molecules;
};

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        const Reaction reaction = Reaction(MathLib::MakeArray<size_t>(
            0, 1, 2, 3
        ), MathLib::MakeArray<size_t>(
            0, 1, 2, 3
        ));
        std::cout << reaction << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}