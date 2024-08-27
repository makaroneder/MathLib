#include <Chemistry/Reaction.hpp>
#include <Tests/Test.hpp>
#include <MathLib.hpp>
#include <iostream>

/// @brief Balances reaction and prints it
/// @tparam T Type of number
/// @param reaction Reaction to balance
template <typename T>
void BalanceReaction(const ChemicalReaction<T>& reaction) {
    const T start = GetTime();
    std::cout << reaction << '\n';
    std::cout << reaction.Balance().Get("Failed to balance chemical reaction") << '\n';
    std::cout << "Reaction balanced in " << GetTime() - start << " second(s)\n";
    for (size_t i = 0; i < 50; i++) std::cout << '-';
    std::cout << '\n';
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        #ifdef Debug
        const Test test = TestSelf();
        const size_t tests = test.GetRecordCount();
        const size_t passed = test.GetPassed();
        std::cout << test << passed << "/" << tests << " tests passed" << std::endl;
        if (passed != tests) Panic("Some tests failed");
        #endif
        // 2KMnO4 + 5KNO2 + 6HNO3 => 2MnN2O6 + 7KNO3 + 3H2O
        BalanceReaction<num_t>(ChemicalReaction<num_t>(std::vector<ChemicalMolecule<num_t>> {
            ChemicalMolecule<num_t>(std::vector<ChemicalElement<num_t>> { Potassium<num_t>(1), Manganese<num_t>(1), Oxygen<num_t>(4), }, 1),
            ChemicalMolecule<num_t>(std::vector<ChemicalElement<num_t>> { Potassium<num_t>(1), Nitrogen<num_t>(1), Oxygen<num_t>(2), }, 1),
            ChemicalMolecule<num_t>(std::vector<ChemicalElement<num_t>> { Hydrogen<num_t>(1), Nitrogen<num_t>(1), Oxygen<num_t>(3), }, 1),
        }, std::vector<ChemicalMolecule<num_t>> {
            ChemicalMolecule<num_t>(std::vector<ChemicalElement<num_t>> { Manganese<num_t>(1), Nitrogen<num_t>(2), Oxygen<num_t>(6), }, 1),
            ChemicalMolecule<num_t>(std::vector<ChemicalElement<num_t>> { Potassium<num_t>(1), Nitrogen<num_t>(1), Oxygen<num_t>(3), }, 1),
            ChemicalMolecule<num_t>(std::vector<ChemicalElement<num_t>> { Hydrogen<num_t>(2), Oxygen<num_t>(1), }, 1),
        }));
        // 2NaOH + H2SO4 => Na2SO4 + 2H2O
        BalanceReaction<num_t>(ChemicalReaction<num_t>::Create(std::vector<ChemicalMolecule<num_t>> {
            ChemicalMolecule<num_t>(std::vector<ChemicalElement<num_t>> { Sodium<num_t>(1), Oxygen<num_t>(1), Hydrogen<num_t>(1), }, 1),
            ChemicalMolecule<num_t>(std::vector<ChemicalElement<num_t>> { Hydrogen<num_t>(2), Sulfur<num_t>(1), Oxygen<num_t>(4), }, 1),
        }).Get("Failed to create chemical reaction"));
        // 2Na + Cl2 => 2NaCl
        BalanceReaction<num_t>(ChemicalReaction<num_t>::Create(std::vector<ChemicalMolecule<num_t>> {
            ChemicalMolecule<num_t>(std::vector<ChemicalElement<num_t>> { Sodium<num_t>(1), }, 1),
            ChemicalMolecule<num_t>(std::vector<ChemicalElement<num_t>> { Chlorine<num_t>(2), }, 1),
        }).Get("Failed to create chemical reaction"));
        // 10Cr7N66H96C42O24 + 1176KMnO4 + 1729H2SO4 => 35K2Cr2O7 + 420CO2 + 660HNO3 + 1176MnSO4 + 1879H2O + 553K2SO4
        BalanceReaction<num_t>(ChemicalReaction<num_t>(std::vector<ChemicalMolecule<num_t>> {
            ChemicalMolecule<num_t>(std::vector<ChemicalElement<num_t>> { Chromium<num_t>(7), Nitrogen<num_t>(66), Hydrogen<num_t>(96), Carbon<num_t>(42), Oxygen<num_t>(24), }, 1),
            ChemicalMolecule<num_t>(std::vector<ChemicalElement<num_t>> { Potassium<num_t>(1), Manganese<num_t>(1), Oxygen<num_t>(4), }, 1),
            ChemicalMolecule<num_t>(std::vector<ChemicalElement<num_t>> { Hydrogen<num_t>(2), Sulfur<num_t>(1), Oxygen<num_t>(4), }, 1),
        }, std::vector<ChemicalMolecule<num_t>> {
            ChemicalMolecule<num_t>(std::vector<ChemicalElement<num_t>> { Potassium<num_t>(2), Chromium<num_t>(2), Oxygen<num_t>(7), }, 1),
            ChemicalMolecule<num_t>(std::vector<ChemicalElement<num_t>> { Carbon<num_t>(1), Oxygen<num_t>(2), }, 1),
            ChemicalMolecule<num_t>(std::vector<ChemicalElement<num_t>> { Hydrogen<num_t>(1), Nitrogen<num_t>(1), Oxygen<num_t>(3), }, 1),
            ChemicalMolecule<num_t>(std::vector<ChemicalElement<num_t>> { Manganese<num_t>(1), Sulfur<num_t>(1), Oxygen<num_t>(4), }, 1),
            ChemicalMolecule<num_t>(std::vector<ChemicalElement<num_t>> { Hydrogen<num_t>(2), Oxygen<num_t>(1), }, 1),
            ChemicalMolecule<num_t>(std::vector<ChemicalElement<num_t>> { Potassium<num_t>(2), Sulfur<num_t>(1), Oxygen<num_t>(4), }, 1),
        }));
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}