#include <Chemistry/Reaction.hpp>
#include <iostream>

int main(void) {
    try {
        // KMnO4 + KNO2 + HNO3 => MnN2O6 + KNO3 + H2O
        ChemicalReaction<num_t> reaction = ChemicalReaction<num_t>({
            ChemicalMolecule<num_t>({ Potassium<num_t>(1), Manganese<num_t>(1), Oxygen<num_t>(4), }, 1),
            ChemicalMolecule<num_t>({ Potassium<num_t>(1), Nitrogen<num_t>(1), Oxygen<num_t>(2), }, 1),
            ChemicalMolecule<num_t>({ Hydrogen<num_t>(1), Nitrogen<num_t>(1), Oxygen<num_t>(3), }, 1),
        }, {
            ChemicalMolecule<num_t>({ Manganese<num_t>(1), Nitrogen<num_t>(2), Oxygen<num_t>(6), }, 1),
            ChemicalMolecule<num_t>({ Potassium<num_t>(1), Nitrogen<num_t>(1), Oxygen<num_t>(3), }, 1),
            ChemicalMolecule<num_t>({ Hydrogen<num_t>(2), Oxygen<num_t>(1), }, 1),
        });
        std::cout << reaction << std::endl;
        if (reaction.BalanceReaction(7)) std::cout << reaction << std::endl;
        else std::cout << "Failed to balance reaction" << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}