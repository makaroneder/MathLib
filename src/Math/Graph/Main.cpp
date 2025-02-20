#define SDL_MAIN_HANDLED
#include <EquationSolver/Preprocesor.hpp>
#include <EquationSolver/Tokenizer.hpp>
#include <EquationSolver/Optimizer.hpp>
#include <Libc/HostFileSystem.hpp>
#include <Libc/HostFunction.hpp>
#include <SDL2.cpp>
#include <iostream>

/// @brief Main loop of this program
/// @tparam T Type of number
/// @param renderer Renderer we are using
/// @param func Function which will be called when the user sends input
/// @param inputSet Set of every input value we allow
/// @param state Current state of the program
/// @param lastState Last state of the program
/// @return Status
template <typename T>
[[nodiscard]] bool HandleEvents(MathLib::Renderer& renderer, const MathLib::Function<bool>& func, size_t& state, const size_t& lastState) {
    bool running = true;
    const T speed = 1;
    if (!func()) return false;
    while (running) {
        if (!renderer.Update()) return false;
        const MathLib::Event event = renderer.GetEvent();
        if (event.type == MathLib::Event::Type::Quit) running = false;
        else if (event.type == MathLib::Event::Type::KeyPressed && event.pressed) {
            bool update = true;
            switch (event.key) {
                case 'w': {
                    GetY(renderer.position) += speed;
                    break;
                }
                case 's': {
                    GetY(renderer.position) -= speed;
                    break;
                }
                case 'd': {
                    GetX(renderer.position) += speed;
                    break;
                }
                case 'a': {
                    GetX(renderer.position) -= speed;
                    break;
                }
                case 'q': {
                    renderer.pointMultiplier -= speed;
                    break;
                }
                case 'e': {
                    renderer.pointMultiplier += speed;
                    break;
                }
                case 'z': {
                    if (state != 0) state--;
                    else state = lastState - 1;
                    break;
                }
                case 'x': {
                    state = (state + 1) % lastState;
                    break;
                }
                default: update = false;
            }
            if (update && !func()) return false;
        }
    }
    return true;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <input file>");
        MathLib::SDL2 sdl2;
        MathLib::HostFileSystem fs;
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Math graph", 800, 800);
        MathLib::Array<MathLib::Optimizer> states;
        for (int i = 1; i < argc; i++) {
            MathLib::Node* root = MathLib::Tokenize(MathLib::Preproces(fs, argv[i]));
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
            if (!states.Add(optimizer)) MathLib::Panic("Failed to add optimizer");
        }
        size_t state = 0;
        const MathLib::HostFunction<MathLib::Array<MathLib::num_t>, MathLib::num_t> func = MathLib::HostFunction<MathLib::Array<MathLib::num_t>, MathLib::num_t>([&states, &state](MathLib::num_t x) -> MathLib::Array<MathLib::num_t> {
            const MathLib::FunctionNode funcNode = states.At(state).GetFunction("f");
            MathLib::Optimizer tmp = states.At(state);
            MathLib::Variable var = MathLib::Variable(funcNode.arguments[0].name, funcNode.arguments[0].dataType, MathLib::ToString(x), true);
            if (!tmp.variables.Add(var)) return MathLib::nan;
            MathLib::Node* n = tmp.Optimize(funcNode.body);
            delete var.value;
            const MathLib::Array<MathLib::complex_t> complexRet = n->ToNumber();
            delete n;
            MathLib::Array<MathLib::num_t> ret = MathLib::Array<MathLib::num_t>(complexRet.GetSize());
            for (size_t i = 0; i < ret.GetSize(); i++) ret.At(i) = complexRet.At(i).ToReal();
            return ret;
        });
        const MathLib::HostFunction<MathLib::complex_t, MathLib::complex_t> complexFunc = MathLib::HostFunction<MathLib::complex_t, MathLib::complex_t>([&states, &state](MathLib::complex_t z) -> MathLib::complex_t {
            const MathLib::FunctionNode funcNode = states.At(state).GetFunction("f");
            MathLib::Optimizer tmp = states.At(state);
            MathLib::Variable var = MathLib::Variable(funcNode.arguments[0].name, funcNode.arguments[0].dataType, z.ToString(), true);
            if (!tmp.variables.Add(var)) return MathLib::complex_t(MathLib::nan, MathLib::nan);
            MathLib::Node* n = tmp.Optimize(funcNode.body);
            delete var.value;
            const MathLib::complex_t ret = n->ToNumber().At(0);
            delete n;
            return ret;
        });
        if (!HandleEvents<MathLib::num_t>(renderer, MathLib::HostFunction<bool>([&renderer, func, complexFunc, &states, &state](void) -> bool {
            renderer.Fill(0);
            renderer.DrawAxis<MathLib::num_t>(0xffffffff, 0x808080ff, 1);
            const MathLib::FunctionNode funcNode = states.At(state).GetFunction("f");
            if (funcNode.dataType == "C")
                renderer.DrawComplexFunction<MathLib::num_t>(renderer.GenerateComplexFunction<MathLib::num_t>(complexFunc));
            else if (funcNode.dataType == "R")
                renderer.DrawFunction<MathLib::num_t>(renderer.GenerateMultiFunction<MathLib::num_t>(func), 0xff0000ff);
            else return false;
            return true;
        }), state, states.GetSize())) MathLib::Panic("Failed to render function");
        for (MathLib::Optimizer& optimizer : states) optimizer.Destroy();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}