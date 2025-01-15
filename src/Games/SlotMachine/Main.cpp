#define SDL_MAIN_HANDLED
#include "Symbol.hpp"
#include "Consumable.hpp"
#include <Libc/HostFileSystem.hpp>
#include <Pair.hpp>
#include <SDL2.cpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        #ifndef Debug
        srand(time(nullptr));
        #endif
        MathLib::HostFileSystem fs;
        const MathLib::String path = "src/TestPrograms/SlotMachine/";
        const Symbol symbols[] = {
            Symbol(fs, path + "Nothing.obj", "Nothing", std::vector<size_t> { 0, 0, 0, }),
            Symbol(fs, path + "Money.obj", "Money", std::vector<size_t> { 2, 5, 10, }),
            Symbol(fs, path + "Bell.obj", "Bell", std::vector<size_t> { 0, 0, 10, }),
            Symbol(fs, path + "Bar.obj", "Bar", std::vector<size_t> { 0, 0, 20, }),
            Symbol(fs, path + "7.obj", "7", std::vector<size_t> { 0, 0, 30, }),
            Symbol(fs, path + "Lemon.obj", "Lemon", std::vector<size_t> { 0, 0, 40, }),
            Symbol(fs, path + "Banana.obj", "Banana", std::vector<size_t> { 0, 0, 60, }),
            Symbol(fs, path + "Orange.obj", "Orange", std::vector<size_t> { 0, 0, 100, }),
        };
        const Consumable consumables[] = {
            Consumable(fs, path + "NormalWater.obj", Consumable::Type::RewardMultiplier, 2, 6000),
            Consumable(fs, path + "PickleWater.obj", Consumable::Type::BetIncreaser, 10, 100),
            Consumable(fs, path + "TapWater.obj", Consumable::Type::BetIncreaser, -10, 100),
            Consumable(fs, path + "FancyWater.obj", Consumable::Type::RewardRankIncreaser, 1, 4000),
        };
        MathLib::WavefrontObject<MathLib::num_t> machine = MathLib::WavefrontObject<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(0, 0, 0), fs, path + "Machine.obj");
        MathLib::SDL2Renderer renderer = MathLib::SDL2Renderer("Slot machine", 800, 800);
        ssize_t score = 100000;
        MathLib::Array<size_t> symbolIndexes = MathLib::Array<size_t>(3);
        MathLib::num_t specialTime = 0;
        MathLib::num_t prevTime = MathLib::GetTime();
        size_t bet = 50;
        size_t multiplier = 1;
        size_t rankIncreaser = 0;
        while (true) {
            const MathLib::num_t currTime = MathLib::GetTime();
            const MathLib::num_t deltaTime = currTime - prevTime;
            prevTime = currTime;
            renderer.Fill(0);
            for (size_t i = 0; i < SizeOfArray(consumables); i++) {
                Consumable tmp = consumables[i];
                const MathLib::num_t y = ((ssize_t)i - 1) * 1.3;
                tmp.position = MathLib::CreateVector<MathLib::num_t>(-3, y, 0);
                if (!renderer.Puts<MathLib::num_t>(MathLib::ToString(SizeOfArray(consumables) - i) + ")", &_binary_src_Lib_zap_light16_psf_start, MathLib::CreateVector<MathLib::num_t>(-3.5, y, 0), 0xff0000ff, 0)) MathLib::Panic("Failed to print text");
                renderer.DrawShape<MathLib::num_t>(tmp, MathLib::CreateVector<MathLib::num_t>(0, 0, 0), 0xff0000ff);
            }
            if (!renderer.Puts<MathLib::num_t>("Score: "_M + MathLib::ToString(score) + "\nBet: " + MathLib::ToString(bet), &_binary_src_Lib_zap_light16_psf_start, MathLib::CreateVector<MathLib::num_t>(0, -0.6, 0), 0xff0000ff, 0)) MathLib::Panic("Failed to print text");
            for (size_t i = 0; i < symbolIndexes.GetSize(); i++) {
                Symbol tmp = symbols[symbolIndexes.At(i)];
                tmp.position = MathLib::CreateVector<MathLib::num_t>(((ssize_t)i - 1) * 0.6, 0.55, 0);
                renderer.DrawShape<MathLib::num_t>(tmp, MathLib::CreateVector<MathLib::num_t>(0, 0, MathLib::IsNaN(specialTime) ? 0 : specialTime), MathLib::IsNaN(specialTime) ? 0x0000ffff : MathLib::GetRainbow<MathLib::num_t>(specialTime));
            }
            renderer.DrawShape<MathLib::num_t>(machine, MathLib::CreateVector<MathLib::num_t>(0, 0, 0), 0x00ff00ff);
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
            const MathLib::Event event = renderer.GetEvent();
            if (event.type == MathLib::Event::Type::Quit) break;
            else if (event.type == MathLib::Event::Type::KeyPressed && event.pressed) {
                switch (event.key) {
                    case ' ': {
                        specialTime = MathLib::MakeNaN();
                        bool special = true;
                        symbolIndexes = MathLib::Array<size_t>(3);
                        for (size_t& i : symbolIndexes) i = MathLib::Round(MathLib::RandomNumber<MathLib::num_t>(0, SizeOfArray(symbols) - 1));
                        for (size_t i = 0; i < symbolIndexes.GetSize() - 1 && special; i++)
                            special = (symbolIndexes.At(i) == symbolIndexes.At(i + 1));
                        if (special) specialTime = 0;
                        MathLib::Array<MathLib::Pair<Symbol, size_t>> scores;
                        for (const size_t& i : symbolIndexes) {
                            bool found = false;
                            for (size_t j = 0; j < scores.GetSize(); j++) {
                                if (scores.At(j).first == symbols[i]) {
                                    scores.At(j).second++;
                                    found = true;
                                    break;
                                }
                            }
                            if (!found && !scores.Add(MathLib::Pair<Symbol, size_t>(symbols[i], 0))) MathLib::Panic("Failed to add score");
                        }
                        score -= bet;
                        for (const MathLib::Pair<Symbol, size_t>& pair : scores) score += multiplier * bet * pair.first.multiplier.At(MathLib::Min<size_t>(pair.second + rankIncreaser, pair.first.multiplier.GetSize()));
                        multiplier = 1;
                        rankIncreaser = 0;
                        break;
                    }
                    case '1' ... '9': {
                        const size_t index = event.key - '1';
                        if (score < consumables[index].cost) break;
                        score -= consumables[index].cost;
                        switch (consumables[index].type) {
                            case Consumable::Type::RewardMultiplier: {
                                multiplier = consumables[index].value;
                                break;
                            }
                            case Consumable::Type::BetIncreaser: {
                                ssize_t tmp = bet + consumables[index].value;
                                if (!tmp) tmp = bet;
                                if (tmp > 10000) tmp = 10000;
                                bet = tmp;
                                break;
                            }
                            case Consumable::Type::RewardRankIncreaser: {
                                rankIncreaser = consumables[index].value;
                                break;
                            }
                        }
                        break;
                    }
                }
            }
            specialTime += deltaTime;
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}