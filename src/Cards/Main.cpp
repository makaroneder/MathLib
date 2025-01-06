#include "Hand.hpp"
#include <MathLib.hpp>
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
        LoadImage(prototype, "Prototype.tga");
        MathLib::Matrix<Card> cards = MathLib::Matrix<Card>((size_t)Card::Type::TypeCount, (size_t)Card::Color::ColorCount);
        for (Card& card : cards) card = Card(&prototype);
        Hand<MathLib::num_t> hand = Hand<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(0, 0, 0));
        SwapCards<MathLib::num_t>(hand, cards);
        bool update = true;
        size_t points = 0;
        while (true) {
            if (update) {
                std::cout << hand << std::endl;
                renderer.Fill(0);
                if (!renderer.Puts<MathLib::num_t>(MathLib::String("Points: ") + MathLib::ToString(points, 10), &_binary_src_Lib_zap_light16_psf_start, MathLib::CreateVector<MathLib::num_t>(0, 3, 0), UINT32_MAX, 0)) MathLib::Panic("Failed to print score");
                if (!hand.Draw(renderer, cards, MathLib::CreateVector<MathLib::num_t>(0, 0, 0))) MathLib::Panic("Failed to draw hand");
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
                        points += hand.GetPoints();
                        hand.SelectAll();
                        SwapCards<MathLib::num_t>(hand, cards);
                        update = true;
                        break;
                    }
                    case 'r': {
                        SwapCards<MathLib::num_t>(hand, cards);
                        update = true;
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