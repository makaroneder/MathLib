#define SDL_MAIN_HANDLED
#define FillGapsInFunctions
#include <MathLib.hpp>
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
bool HandleEvents(Renderer& renderer, const Function<bool>& func, size_t& state, const size_t& lastState) {
    bool running = true;
    const T speed = 1;
    if (!func()) return false;
    while (running) {
        const Event event = renderer.GetEvent();
        if (event.type == Event::Type::Quit) running = false;
        else if (event.type == Event::Type::KeyPressed) {
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
        #ifdef Debug
        const Test test = TestSelf();
        const size_t tests = test.GetRecordCount();
        const size_t passed = test.GetPassed();
        std::cout << test << passed << "/" << tests << " tests passed" << std::endl;
        if (passed != tests) Panic("Some tests failed");
        #endif
        if (argc < 2) Panic(String("Usage: ") + argv[0] + " <input file>");
        HostFileSystem fs;
        SDL2Renderer renderer = SDL2Renderer("Math graph", 800, 800);
        Array<Optimizer> states;
        for (int i = 1; i < argc; i++) {
            Node* root = Tokenize(Preproces(fs, argv[i]));
            #ifdef Debug
            std::cout << "Generated nodes:\n" << *root << std::endl;
            #endif
            Optimizer optimizer = Optimizer();
            Node* optimizedRoot = optimizer.Optimize(root);
            delete root;
            #ifdef Debug
            std::cout << "Optimized nodes:\n" << *optimizedRoot << std::endl;
            #endif
            delete optimizedRoot;
            optimizer.runtime = true;
            states.Add(optimizer);
        }
        size_t state = 0;
        const HostFunction<Array<num_t>, num_t> func = HostFunction<Array<num_t>, num_t>(nullptr, [&states, &state](const void*, num_t x) -> Array<num_t> {
            const FunctionNode funcNode = states.At(state).GetFunction("f");
            Optimizer tmp = states.At(state);
            Variable var = Variable(funcNode.arguments[0].name, funcNode.arguments[0].dataType, ToString(x), true);
            tmp.variables.Add(var);
            Node* n = tmp.Optimize(funcNode.body);
            delete var.value;
            const Array<complex_t> complexRet = n->ToNumber();
            delete n;
            Array<num_t> ret = Array<num_t>(complexRet.GetSize());
            for (size_t i = 0; i < ret.GetSize(); i++) ret.At(i) = !complexRet.At(i).GetImaginary() ? complexRet.At(i).GetReal() : MakeNaN();
            return ret;
        });
        const HostFunction<complex_t, complex_t> complexFunc = HostFunction<complex_t, complex_t>(nullptr, [&states, &state](const void*, complex_t z) -> complex_t {
            const FunctionNode funcNode = states.At(state).GetFunction("f");
            Optimizer tmp = states.At(state);
            Variable var = Variable(funcNode.arguments[0].name, funcNode.arguments[0].dataType, z.ToString(), true);
            tmp.variables.Add(var);
            Node* n = tmp.Optimize(funcNode.body);
            delete var.value;
            const complex_t ret = n->ToNumber().At(0);
            delete n;
            return ret;
        });
        if (!HandleEvents<num_t>(renderer, HostFunction<bool>(nullptr, [&renderer, func, complexFunc, &states, &state](const void*) -> bool {
            renderer.Fill(0);
            renderer.DrawAxis(0xffffffff, 0x808080ff);
            const FunctionNode funcNode = states.At(state).GetFunction("f");
            if (funcNode.dataType == "C")
                renderer.DrawComplexFunction<num_t>(renderer.GenerateComplexFunction<num_t>(complexFunc));
            else if (funcNode.dataType == "R")
                renderer.DrawFunction<num_t>(renderer.GenerateMultiFunction<num_t>(func), 0xff0000ff);
            else return false;
            return renderer.Update();
        }), state, states.GetSize())) Panic("Failed to render function");
        for (Optimizer& optimizer : states) optimizer.Destroy();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}