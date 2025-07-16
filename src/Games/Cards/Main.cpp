#define SDL_MAIN_HANDLED
#include "Hand.hpp"
#include <EquationSolver/Preprocesor.hpp>
#include <Allocator/RegionAllocator.hpp>
#include <EquationSolver/Optimizer.hpp>
#include <EquationSolver/Tokenizer.hpp>
#include <Image/Aseprite/Aseprite.hpp>
#include <Libc/HostFileSystem.hpp>
#include <Bitmap.hpp>
#include <SDL2.cpp>
#include <iostream>

template <typename T>
[[nodiscard]] bool SwapCards(Hand<T>& hand, MathLib::Bitmap& cards) {
    for (size_t i = 0; i < Hand<T>::size; i++) {
        if (!hand.ShouldSwap(i)) continue;
        size_t x;
        size_t y;
        while (true) {
            x = MathLib::RandomNumber<size_t>(0, (size_t)Card::Type::TypeCount);
            y = MathLib::RandomNumber<size_t>(0, (size_t)Card::Color::ColorCount);
            const MathLib::Expected<bool> tmp = cards.Get(y * (size_t)Card::Type::TypeCount + x);
            if (!tmp.HasValue()) return false;
            if (tmp.Get()) break;
        }
        if (!cards.Set(y * (size_t)Card::Type::TypeCount + x, false)) return false;
        const Card card = hand.Swap(Card((Card::Type)x, (Card::Color)y), i);
        if (card.type != Card::Type::TypeCount && !cards.Set((size_t)card.color * (size_t)Card::Type::TypeCount + (size_t)card.type, true)) return false;
    }
    return true;
}
[[nodiscard]] size_t FindVariable(const MathLib::Sequence<MathLib::Variable>& variables, const MathLib::Sequence<char>& name) {
    for (size_t i = 0; i < variables.GetSize(); i++)
        if (name == variables.At(i).name) return i;
    return SIZE_MAX;
}
template <typename T>
[[nodiscard]] T Evaluate(MathLib::Optimizer& optimizer, const MathLib::FunctionNode& func, size_t variable, const MathLib::Sequence<char>& value) {
    optimizer.variables.At(variable).value->value = MathLib::CollectionToString(value);
    MathLib::Node* tmp = optimizer.Optimize(func.body);
    const MathLib::Array<MathLib::complex_t> tmpArr = tmp->ToNumber();
    const T ret = tmpArr.At(0).ToReal();
    delete tmp;
    return ret;
}
#define AddFunction(vname, identifier)                                                                                                                                      \
const MathLib::FunctionNode vname = optimizer.GetFunction(identifier);                                                                                                      \
    size_t vname##Variable = FindVariable(optimizer.variables, vname.arguments.At(0).name);                                                                                 \
    if (vname##Variable == SIZE_MAX) {                                                                                                                                      \
        if (!optimizer.variables.Add(MathLib::Variable(vname.arguments.At(0).name, vname.arguments.At(0).dataType, '0'_M, true))) MathLib::Panic("Failed to add variable"); \
        vname##Variable = optimizer.variables.GetSize() - 1;                                                                                                                \
    }
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::allocator = new MathLib::RegionAllocator(MathLib::allocator, 1024 * 1024);
        if (!MathLib::allocator) MathLib::Panic("Failed to allocate allocator");
        #ifndef Debug
        srand(time(nullptr));
        #endif
        MathLib::SDL2 sdl2;
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Cards", 800, 800);
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
        AddFunction(hands, "hands"_M)
        AddFunction(discards, "discards"_M)
        AddFunction(requiredPoints, "points"_M)
        MathLib::Aseprite table;
        if (!table.LoadFromPath(fs, path + "Table.aseprite")) MathLib::Panic("Failed to load table image");
        MathLib::Aseprite cardsImage;
        if (!cardsImage.LoadFromPath(fs, path + "Cards.aseprite")) MathLib::Panic("Failed to load cards");
        MathLib::Bitmap cards = MathLib::Bitmap((size_t)Card::Type::TypeCount * (size_t)Card::Color::ColorCount);
        cards.Fill(true);
        Hand<MathLib::num_t> hand = Hand<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(0, -1, 0));
        if (!SwapCards<MathLib::num_t>(hand, cards)) MathLib::Panic("Failed to initialize deck");
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
                renderer.DrawImage<MathLib::num_t>(table.At(0), MathLib::CreateVector<MathLib::num_t>(0, 0, 0));
                if (!renderer.Puts<MathLib::num_t>("Round: "_M + MathLib::ToString(round, 10), MathLib::zap_light16_psf, MathLib::CreateVector<MathLib::num_t>(0.05, 3.7, 0), UINT32_MAX, 0)) MathLib::Panic("Failed to print state");
                if (!renderer.Puts<MathLib::num_t>("Remaining hands: "_M + MathLib::ToString(remainingHands, 10), MathLib::zap_light16_psf, MathLib::CreateVector<MathLib::num_t>(0.05, 3.5, 0), UINT32_MAX, 0)) MathLib::Panic("Failed to print state");
                if (!renderer.Puts<MathLib::num_t>("Remaining discards: "_M + MathLib::ToString(remainingDiscards, 10), MathLib::zap_light16_psf, MathLib::CreateVector<MathLib::num_t>(0.05, 3.3, 0), UINT32_MAX, 0)) MathLib::Panic("Failed to print state");
                if (!renderer.Puts<MathLib::num_t>("Remaining points: "_M + MathLib::ToString(remainingPoints, 10), MathLib::zap_light16_psf, MathLib::CreateVector<MathLib::num_t>(0.05, 3.1, 0), UINT32_MAX, 0)) MathLib::Panic("Failed to print state");
                if (!hand.Draw(renderer, cardsImage)) MathLib::Panic("Failed to draw hand");
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
                        if (!SwapCards<MathLib::num_t>(hand, cards)) MathLib::Panic("Failed to play hand");
                        update = true;
                        break;
                    }
                    case 'r': {
                        if (remainingDiscards) {
                            remainingDiscards--;
                            if (!SwapCards<MathLib::num_t>(hand, cards)) MathLib::Panic("Failed to discard cards");
                            update = true;
                        }
                        break;
                    }
                }
            }
        }
        delete MathLib::allocator;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}