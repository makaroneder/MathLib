#define FillGapsInFunctions
#include <SDL2.cpp>
#include <Libc/HostFileSystem.hpp>
#include <EquationSolver/Optimizer.hpp>
#include <EquationSolver/Tokenizer.hpp>
#include <EquationSolver/Preprocesor.hpp>

/// @brief Main loop of this program
/// @tparam T Type of number
/// @param renderer Renderer we are using
/// @param func Function which will be called when the user sends input
/// @param inputSet Set of every input value we allow
/// @param state Current state of the program
/// @param lastState Last state of the program
/// @return Status
template <typename T>
bool HandleEvents(Renderer& renderer, std::function<bool(void)> func, Array<T>& inputSet, size_t& state, size_t lastState) {
    bool running = true;
    const T speed = 1;
    if (!func()) return false;
    while (running) {
        const Event event = renderer.GetEvent();
        if (event.type == Event::Type::Quit) running = false;
        else if (event.type == Event::Type::KeyPressed) {
            bool update = true;
            switch (event.data.key) {
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
                    for (size_t i = 0; i < inputSet.GetSize(); i++) inputSet.At(i) += speed;
                    break;
                }
                case 'a': {
                    if (GetX(renderer.position) >= (-17 + speed)) {
                        GetX(renderer.position) -= speed;
                        for (size_t i = 0; i < inputSet.GetSize(); i++) inputSet.At(i) -= speed;
                    }
                    break;
                }
                case 'q': {
                    if (renderer.pointMultiplier > (9 + speed) && FloatsEqual<T>(GetX(renderer.position), 0) && FloatsEqual<T>(GetY(renderer.position), 0)) {
                        renderer.pointMultiplier -= speed;
                        inputSet = renderer.CreateRealNumberSet<T>();
                    }
                    break;
                }
                case 'e': {
                    if (FloatsEqual<T>(GetX(renderer.position), 0) && FloatsEqual<T>(GetY(renderer.position), 0)) {
                        renderer.pointMultiplier += speed;
                        inputSet = renderer.CreateRealNumberSet<T>();
                    }
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
        if (argc < 2) Panic("No input files provided");
        HostFileSystem fs;
        SDL2Renderer renderer = SDL2Renderer("Math graph", 800, 800);
        Array<num_t> inputSet = renderer.CreateRealNumberSet<num_t>();
        Array<State> states;
        for (int i = 1; i < argc; i++) {
            Node* root = Tokenize(Preproces(fs, argv[i]));
            #ifdef Debug
            std::cout << "Generated nodes:\n" << *root << std::endl;
            #endif
            State state = State();
            Node* optimizedRoot = Optimize(root, state);
            delete root;
            #ifdef Debug
            std::cout << "Optimized nodes:\n" << *optimizedRoot << std::endl;
            #endif
            delete optimizedRoot;
            states.Add(state);
        }
        size_t state = 0;
        const auto func = [&states, &state](num_t x) -> Array<num_t> {
            const Function funcNode = states.At(state).GetFunction("f");
            State tmp = states.At(state);
            tmp.variables.Add(Variable(funcNode.arguments[0].name, std::to_string(x)));
            Node* n = Optimize(funcNode.body, tmp);
            const Array<complex_t> complexRet = n->ToNumber();
            delete n;
            Array<num_t> ret = Array<num_t>(complexRet.GetSize());
            for (size_t i = 0; i < ret.GetSize(); i++) ret.At(i) = complexRet.At(i).imag() == 0 ? complexRet.At(i).real() : NAN;
            return ret;
        };
        const auto complexFunc = [&states, &state](complex_t z) -> complex_t {
            const Function funcNode = states.At(state).GetFunction("f");
            State tmp = states.At(state);
            tmp.variables.Add(Variable(funcNode.arguments[0].name, ComplexToString<num_t>(z)));
            Node* n = Optimize(funcNode.body, tmp);
            const complex_t ret = n->ToNumber().At(0);
            delete n;
            return ret;
        };
        if (!HandleEvents<num_t>(renderer, [&renderer, func, complexFunc, &inputSet, &states, &state](void) {
            renderer.Fill(0x000000ff);
            renderer.DrawAxis(0xffffffff, 0x808080ff);
            const Function funcNode = states.At(state).GetFunction("f");
            if (funcNode.domain == "C" && funcNode.codomain == "C")
                renderer.DrawComplexFunction<num_t>(renderer.GenerateComplexFunction<num_t>(complexFunc, inputSet));
            else if (funcNode.domain == "R" && funcNode.codomain == "R")
                renderer.DrawFunction<num_t>(renderer.GenerateMultiFunction<num_t>(func, inputSet), 0xff0000ff);
            else return false;
            return renderer.Update();
        }, inputSet, state, states.GetSize())) Panic("Failed to render function");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}