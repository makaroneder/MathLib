#include "Symbol.hpp"
#include <Physics/SIUnits.hpp>
#include <SDL2.cpp>
#include <iostream>

int main(void) {
    try {
        #ifndef Debug
        srand(time(nullptr));
        #endif
        const std::string path = "src/TestPrograms/Gambling/";
        std::vector<Symbol> symbols = {
            Symbol(path + "Nothing.obj", "Nothing", { 0, 0, 0, }),
            Symbol(path + "Money.obj", "Money", { 2, 5, 10, }),
            Symbol(path + "Bell.obj", "Bell", { 0, 0, 10, }),
            Symbol(path + "Bar.obj", "Bar", { 0, 0, 20, }),
            Symbol(path + "7.obj", "7", { 0, 0, 30, }),
            Symbol(path + "Lemon.obj", "Lemon", { 0, 0, 40, }),
            Symbol(path + "Banana.obj", "Banana", { 0, 0, 60, }),
            Symbol(path + "Orange.obj", "Orange", { 0, 0, 100, }),
        };
        WavefrontObject<num_t> machine = WavefrontObject<num_t>(CreateVector<num_t>(0, 0, 0), path + "Machine.obj");
        SDL2Renderer renderer = SDL2Renderer("Gambling", 800, 800);
        ssize_t score = 0;
        const size_t bet = 50;
        std::vector<size_t> symbolIndexes = std::vector<size_t>(3, 0);
        Second<num_t> specialTime = 0;
        Second<num_t> prevTime = (num_t)clock() / CLOCKS_PER_SEC;
        while (true) {
            const Second<num_t> currTime = (num_t)clock() / CLOCKS_PER_SEC;
            const Second<num_t> deltaTime = currTime - prevTime;
            renderer.Fill(0x000000ff);
            renderer.Puts({ "Score: " + std::to_string(score), "Bet: " + std::to_string(bet) }, &_binary_src_Lib_zap_light16_psf_start, CreateVector<num_t>(0, -0.6, 0), CreateVector<num_t>(0, 0, 0), CreateVector<size_t>(1, 1, 1), 0xff0000ff);
            for (size_t i = 0; i < symbolIndexes.size(); i++) {
                Symbol tmp = symbols[symbolIndexes[i]];
                tmp.position = CreateVector<num_t>(((ssize_t)i - 1) * 0.6, 0.55, 0);
                renderer.DrawShape<num_t>(tmp, CreateVector<num_t>(0, 0, 0), IsNaN(specialTime.GetValue()) ? 0x0000ffff : GetRainbow<num_t>(specialTime.GetValue()));
            }
            renderer.DrawShape<num_t>(machine, CreateVector<num_t>(0, 0, 0), 0x00ff00ff);
            renderer.Update();
            const Event event = renderer.GetEvent();
            if (event.type == Event::Type::Quit) break;
            else if (event.type == Event::Type::KeyPressed && event.data.key == ' ') {
                specialTime = NAN;
                bool special = true;
                symbolIndexes = std::vector<size_t>(3);
                for (size_t i = 0; i < 3; i++) symbolIndexes.at(i) = std::round(RandomNumber<num_t>(0, symbols.size() - 1));
                for (size_t i = 0; i < symbolIndexes.size() - 1 && special; i++)
                    special = (symbolIndexes[i] == symbolIndexes[i + 1]);
                if (special) specialTime = 0;
                std::vector<std::pair<Symbol, size_t>> scores;
                for (const size_t& index : symbolIndexes) {
                    bool found = false;
                    for (std::pair<Symbol, size_t>& pair : scores) {
                        if (pair.first == symbols[index]) {
                            pair.second++;
                            found = true;
                            break;
                        }
                    }
                    if (!found) scores.push_back(std::make_pair(symbols[index], 0));
                }
                score -= bet;
                for (const std::pair<Symbol, size_t>& pair : scores) score += bet * pair.first.multiplier[pair.second];
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