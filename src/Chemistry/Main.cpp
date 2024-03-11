#include <ChemicalElements.hpp>
#include <iostream>

int main(void) {
    try {
        ChemicalReaction<num_t> reaction = ChemicalReaction<num_t>({
            ChemicalMolecule<num_t>({ Potassium<num_t>(1), Manganese<num_t>(1), Oxygen<num_t>(4), }, 1),
            ChemicalMolecule<num_t>({ Potassium<num_t>(1), Nitrogen<num_t>(1), Oxygen<num_t>(2), }, 1),
            ChemicalMolecule<num_t>({ Hydrogen<num_t>(1), Nitrogen<num_t>(1), Oxygen<num_t>(3), }, 1),
        }, {
            ChemicalMolecule<num_t>({ Manganese<num_t>(1), Nitrogen<num_t>(2), Oxygen<num_t>(6), }, 1),
            ChemicalMolecule<num_t>({ Potassium<num_t>(1), Nitrogen<num_t>(1), Oxygen<num_t>(3), }, 1),
            ChemicalMolecule<num_t>({ Hydrogen<num_t>(2), Oxygen<num_t>(1), }, 1),
        });
        std::cout << reaction.ToString() << std::endl;
        if (reaction.BalanceReaction(7)) std::cout << reaction.ToString() << std::endl;
        else std::cout << "Failed to balance reaction" << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}