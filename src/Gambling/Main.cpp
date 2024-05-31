#include "Symbol.hpp"
#include <Physics/SIUnits.hpp>
#include <Libc/HostFileSystem.hpp>
#include <SDL2.cpp>

int main(void) {
    try {
        #ifndef Debug
        srand(time(nullptr));
        #endif
        HostFileSystem fs;
        const String path = "src/TestPrograms/Gambling/";
        Array<Symbol> symbols = std::vector<Symbol> {
            Symbol(fs, path + "Nothing.obj", "Nothing", std::vector<size_t> { 0, 0, 0, }),
            Symbol(fs, path + "Money.obj", "Money", std::vector<size_t> { 2, 5, 10, }),
            Symbol(fs, path + "Bell.obj", "Bell", std::vector<size_t> { 0, 0, 10, }),
            Symbol(fs, path + "Bar.obj", "Bar", std::vector<size_t> { 0, 0, 20, }),
            Symbol(fs, path + "7.obj", "7", std::vector<size_t> { 0, 0, 30, }),
            Symbol(fs, path + "Lemon.obj", "Lemon", std::vector<size_t> { 0, 0, 40, }),
            Symbol(fs, path + "Banana.obj", "Banana", std::vector<size_t> { 0, 0, 60, }),
            Symbol(fs, path + "Orange.obj", "Orange", std::vector<size_t> { 0, 0, 100, }),
        };
        WavefrontObject<num_t> machine = WavefrontObject<num_t>(CreateVector<num_t>(0, 0, 0), fs, path + "Machine.obj");
        SDL2Renderer renderer = SDL2Renderer("Gambling", 800, 800);
        ssize_t score = 0;
        const size_t bet = 50;
        Array<size_t> symbolIndexes = Array<size_t>(3);
        Second<num_t> specialTime = 0;
        Second<num_t> prevTime = (num_t)clock() / CLOCKS_PER_SEC;
        while (true) {
            const Second<num_t> currTime = (num_t)clock() / CLOCKS_PER_SEC;
            const Second<num_t> deltaTime = currTime - prevTime;
            renderer.Fill(0x000000ff);
            if (!renderer.Puts(std::vector<String> { "Score: " + std::to_string(score), "Bet: " + std::to_string(bet) }, &_binary_src_Lib_zap_light16_psf_start, CreateVector<num_t>(0, -0.6, 0), CreateVector<num_t>(0, 0, 0), CreateVector<size_t>(1, 1, 1), 0xff0000ff)) Panic("Failed to print text");
            for (size_t i = 0; i < symbolIndexes.GetSize(); i++) {
                Symbol tmp = symbols.At(symbolIndexes.At(i));
                tmp.position = CreateVector<num_t>(((ssize_t)i - 1) * 0.6, 0.55, 0);
                renderer.DrawShape<num_t>(tmp, CreateVector<num_t>(0, 0, 0), IsNaN(specialTime.GetValue()) ? 0x0000ffff : GetRainbow<num_t>(specialTime.GetValue()));
            }
            renderer.DrawShape<num_t>(machine, CreateVector<num_t>(0, 0, 0), 0x00ff00ff);
            if (!renderer.Update()) Panic("Failed to update UI");
            const Event event = renderer.GetEvent();
            if (event.type == Event::Type::Quit) break;
            else if (event.type == Event::Type::KeyPressed && event.data.key == ' ') {
                specialTime = NAN;
                bool special = true;
                symbolIndexes = Array<size_t>(3);
                for (size_t i = 0; i < 3; i++) symbolIndexes.At(i) = Round(RandomNumber<num_t>(0, symbols.GetSize() - 1));
                for (size_t i = 0; i < symbolIndexes.GetSize() - 1 && special; i++)
                    special = (symbolIndexes.At(i) == symbolIndexes.At(i + 1));
                if (special) specialTime = 0;
                Array<std::pair<Symbol, size_t>> scores;
                for (size_t i = 0; i < symbolIndexes.GetSize(); i++) {
                    bool found = false;
                    for (size_t j = 0; j < scores.GetSize(); j++) {
                        if (scores.At(j).first == symbols.At(symbolIndexes.At(i))) {
                            scores.At(j).second++;
                            found = true;
                            break;
                        }
                    }
                    if (!found) scores.Add(std::make_pair(symbols.At(symbolIndexes.At(i)), 0));
                }
                score -= bet;
                for (size_t i = 0; i < scores.GetSize(); i++) score += bet * scores.At(i).first.multiplier.At(scores.At(i).second);
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