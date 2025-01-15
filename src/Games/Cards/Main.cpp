#include "Hand.hpp"
#include <EquationSolver/Preprocesor.hpp>
#include <EquationSolver/Optimizer.hpp>
#include <EquationSolver/Tokenizer.hpp>
#include <Libc/HostFileSystem.hpp>
#include <Image/TGA/TGA.hpp>
#include <SDL2.cpp>
#include <iostream>

template <typename T>
void SwapCards(Hand<T>& hand, MathLib::Matrix<Card>& cards) {
    for (size_t i = 0; i < Hand<T>::size; i++) {
        if (hand.ShouldSwap(i)) {
            size_t x;
            size_t y;
            while (true) {
                x = MathLib::RandomNumber<size_t>(0, (size_t)Card::Type::TypeCount);
                y = MathLib::RandomNumber<size_t>(0, (size_t)Card::Color::ColorCount);
                if (cards.At(x, y).free) break;
            }
            cards.At(x, y).free = false;
            const CardIndex card = hand.Swap(CardIndex((Card::Type)x, (Card::Color)y), i);
            if (card.type != Card::Type::TypeCount) cards.At((size_t)card.type, (size_t)card.color).free = true;
        }
    }
}
[[nodiscard]] size_t FindVariable(const MathLib::Array<MathLib::Variable>& variables, const MathLib::String& name) {
    for (size_t i = 0; i < variables.GetSize(); i++)
        if (name == variables.At(i).name) return i;
    return SIZE_MAX;
}
template <typename T>
[[nodiscard]] T Evaluate(MathLib::Optimizer& optimizer, const MathLib::FunctionNode& func, size_t variable, const MathLib::String& value) {
    optimizer.variables.At(variable).value->value = value;
    MathLib::Node* tmp = optimizer.Optimize(func.body);
    const MathLib::Array<MathLib::complex_t> tmpArr = tmp->ToNumber();
    const T ret = tmpArr.At(0);
    delete tmp;
    return ret;
}
#define AddFunction(vname, identifier)                                                                                                                                      \
    MathLib::FunctionNode vname = optimizer.GetFunction(identifier);                                                                                                        \
    size_t vname##Variable = FindVariable(optimizer.variables, vname.arguments.At(0).name);                                                                                 \
    if (vname##Variable == SIZE_MAX) {                                                                                                                                      \
        if (!optimizer.variables.Add(MathLib::Variable(vname.arguments.At(0).name, vname.arguments.At(0).dataType, "0", true))) MathLib::Panic("Failed to add variable");   \
        vname##Variable = optimizer.variables.GetSize() - 1;                                                                                                                \
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
        MathLib::SDL2Renderer renderer = MathLib::SDL2Renderer("Cards", 800, 800);
        MathLib::HostFileSystem fs;
        const MathLib::String path = "src/TestPrograms/Cards/";
        MathLib::Node* root = MathLib::Tokenize(MathLib::Preproces(fs, path + "Program.txt"));
        #ifdef Debug
        std::cout << "Generated nodes:\n" << *root << std::endl;
        #endif
        MathLib::Optimizer optimizer = MathLib::Optimizer();
        MathLib::Node* optimizedRoot = optimizer.Optimize(root);
        delete root;
        #ifdef Debug
        std::cout << "Optimized nodes:\n" << *optimizedRoot << std::endl;
        #endif
        delete optimizedRoot;
        optimizer.runtime = true;
        AddFunction(hands, "hands")
        AddFunction(discards, "discards")
        AddFunction(requiredPoints, "points")
        MathLib::Matrix<Card> cards = MathLib::Matrix<Card>((size_t)Card::Type::TypeCount, (size_t)Card::Color::ColorCount);
        for (size_t color = 0; color < cards.GetHeight(); color++) {
            for (size_t type = 0; type < cards.GetWidth(); type++) {
                MathLib::String str = "";
                switch ((Card::Color)color) {
                    case Card::Color::Clubs: {
                        str = "Clubs";
                        break;
                    }
                    case Card::Color::Diamonds: {
                        str = "Diamonds";
                        break;
                    }
                    case Card::Color::Hearts: {
                        str = "Hearts";
                        break;
                    }
                    case Card::Color::Spades: {
                        str = "Spades";
                        break;
                    }
                    default: MathLib::Panic("Failed to load hand");
                }
                str += "/";
                switch ((Card::Type)type) {
                    case Card::Type::C2: {
                        str += "2";
                        break;
                    }
                    case Card::Type::C3: {
                        str += "3";
                        break;
                    }
                    case Card::Type::C4: {
                        str += "4";
                        break;
                    }
                    case Card::Type::C5: {
                        str += "5";
                        break;
                    }
                    case Card::Type::C6: {
                        str += "6";
                        break;
                    }
                    case Card::Type::C7: {
                        str += "7";
                        break;
                    }
                    case Card::Type::C8: {
                        str += "8";
                        break;
                    }
                    case Card::Type::C9: {
                        str += "9";
                        break;
                    }
                    case Card::Type::C10: {
                        str += "10";
                        break;
                    }
                    case Card::Type::Jack: {
                        str += "Jack";
                        break;
                    }
                    case Card::Type::Queen: {
                        str += "Queen";
                        break;
                    }
                    case Card::Type::King: {
                        str += "King";
                        break;
                    }
                    case Card::Type::Ace: {
                        str += "Ace";
                        break;
                    }
                    default: MathLib::Panic("Failed to load hand");
                }
                MathLib::TGA* tga = new MathLib::TGA(0, 0);
                if (!tga || !tga->LoadFromPath(fs, path + str + ".tga")) MathLib::Panic("Failed to load image");
                cards.At(type, color) = Card(tga);
            }
        }
        Hand<MathLib::num_t> hand = Hand<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(0, 0, 0));
        SwapCards<MathLib::num_t>(hand, cards);
        bool update = true;
        size_t round = 0;
        size_t remainingHands = 0;
        size_t remainingDiscards = 0;
        ssize_t remainingPoints = 0;
        while (true) {
            if (!remainingPoints) {
                round++;
                remainingHands = Evaluate<size_t>(optimizer, hands, handsVariable, MathLib::ToString(round, 10));
                remainingDiscards = Evaluate<size_t>(optimizer, discards, discardsVariable, MathLib::ToString(round, 10));
                remainingPoints = Evaluate<size_t>(optimizer, requiredPoints, requiredPointsVariable, MathLib::ToString(round, 10));
                update = true;
            }
            if (!remainingHands) {
                round = remainingPoints = 0;
                continue;
            }
            if (update) {
                renderer.Fill(0);
                if (!renderer.Puts<MathLib::num_t>("Round: "_M + MathLib::ToString(round, 10), &_binary_src_Lib_zap_light16_psf_start, MathLib::CreateVector<MathLib::num_t>(0, 3.6, 0), UINT32_MAX, 0)) MathLib::Panic("Failed to print state");
                if (!renderer.Puts<MathLib::num_t>("Remaining hands: "_M + MathLib::ToString(remainingHands, 10), &_binary_src_Lib_zap_light16_psf_start, MathLib::CreateVector<MathLib::num_t>(0, 3.4, 0), UINT32_MAX, 0)) MathLib::Panic("Failed to print state");
                if (!renderer.Puts<MathLib::num_t>("Remaining discards: "_M + MathLib::ToString(remainingDiscards, 10), &_binary_src_Lib_zap_light16_psf_start, MathLib::CreateVector<MathLib::num_t>(0, 3.2, 0), UINT32_MAX, 0)) MathLib::Panic("Failed to print state");
                if (!renderer.Puts<MathLib::num_t>("Remaining points: "_M + MathLib::ToString(remainingPoints, 10), &_binary_src_Lib_zap_light16_psf_start, MathLib::CreateVector<MathLib::num_t>(0, 3, 0), UINT32_MAX, 0)) MathLib::Panic("Failed to print state");
                if (!hand.Draw(renderer, cards)) MathLib::Panic("Failed to draw hand");
                update = false;
            }
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
            const MathLib::Event event = renderer.GetEvent();
            if (event.type == MathLib::Event::Type::Quit) break;
            else if (event.type == MathLib::Event::Type::MousePressed && event.pressed && event.mouseButton == MathLib::Event::MouseButton::Left)
                update = hand.Select(renderer.IndexToPosition(MathLib::ConvertMatrix<size_t, MathLib::num_t>(event.mousePosition)));
            else if (event.type == MathLib::Event::Type::KeyPressed && event.pressed) {
                switch (event.key) {
                    case 'e': {
                        remainingPoints -= hand.GetPoints();
                        remainingHands--;
                        if (remainingPoints < 0) remainingPoints = 0;
                        hand.SelectAll();
                        SwapCards<MathLib::num_t>(hand, cards);
                        update = true;
                        break;
                    }
                    case 'r': {
                        if (remainingDiscards) {
                            remainingDiscards--;
                            SwapCards<MathLib::num_t>(hand, cards);
                            update = true;
                        }
                        break;
                    }
                }
            }
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}