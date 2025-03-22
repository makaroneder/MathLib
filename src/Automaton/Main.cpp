#include "FiniteStateMachine.hpp"
#include "BusyBeaver.hpp"
#include <Automaton/Language.hpp>
#include <String.hpp>
#include <iostream>

template <typename Input, typename Output, typename State>
void TestAutomaton(const MathLib::Sequence<char>& automatonName, const MathLib::Automaton<Input, Output, State>& automaton, const MathLib::Language<Input>& language, size_t size) {
    bool recognized = true;
    for (size_t i = 0; i < size; i++) {
        const MathLib::Expected<bool> tmp = automaton.Recognizes(language);
        if (!tmp.Get("Failed to run automaton with given language")) {
            recognized = false;
            break;
        }
    }
    std::cout << "Automaton: " << MathLib::CollectionToString(automatonName) << std::endl;
    std::cout << "Language: " << MathLib::ToString<MathLib::Array<Input>>(language) << std::endl;
    std::cout << "Recognized: " << MathLib::BoolToString(recognized) << std::endl;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        const size_t size = 10;
        TestAutomaton<uint8_t, uint8_t, uint8_t>("Finite state machine"_M, FiniteStateMachine(), MathLib::Language<uint8_t>(MathLib::MakeArray<MathLib::Array<uint8_t>>(
            MathLib::MakeArray<uint8_t>(1),
            MathLib::MakeArray<uint8_t>(0, 0)
        )), size);
        TestAutomaton<uint8_t, uint8_t, uint8_t>("Busy beaver"_M, BusyBeaver(), MathLib::Language<uint8_t>(MathLib::MakeArray<MathLib::Array<uint8_t>>(
            MathLib::MakeArray<uint8_t>(1),
            MathLib::MakeArray<uint8_t>(0)
        )), size);
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}