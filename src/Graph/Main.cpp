#define FillGapsInFunctions
#include <SDL2.hpp>
#include <EquationSolver/Optimizer.hpp>
#include <EquationSolver/Tokenizer.hpp>
#include <EquationSolver/Preprocesor.hpp>
#include <iostream>

/// @brief Main loop of this program
/// @param renderer Renderer we are using
/// @param func Function which will be called when the user sends input
/// @param inputSet Set of every input value we allow
/// @param state Current state of the program
/// @param lastState Last state of the program
/// @return Status
bool HandleEvents(Renderer& renderer, std::function<bool(void)> func, std::vector<num_t>& inputSet, size_t& state, size_t lastState) {
    bool running = true;
    const num_t speed = 1;
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
                    for (num_t& x : inputSet) x += speed;
                    break;
                }
                case 'a': {
                    if (GetX(renderer.position) >= (-17 + speed)) {
                        GetX(renderer.position) -= speed;
                        for (num_t& x : inputSet) x -= speed;
                    }
                    break;
                }
                case 'q': {
                    if (renderer.pointMultiplier > (9 + speed) && FloatsEqual<num_t>(GetX(renderer.position), 0) && FloatsEqual<num_t>(GetY(renderer.position), 0)) {
                        renderer.pointMultiplier -= speed;
                        inputSet = renderer.CreateRealNumberSet<num_t>();
                    }
                    break;
                }
                case 'e': {
                    if (FloatsEqual<num_t>(GetX(renderer.position), 0) && FloatsEqual<num_t>(GetY(renderer.position), 0)) {
                        renderer.pointMultiplier += speed;
                        inputSet = renderer.CreateRealNumberSet<num_t>();
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
        if (argc < 2) throw std::runtime_error("No input files provided");
        SDL2Renderer renderer = SDL2Renderer("Math graph", 800, 800);
        std::vector<num_t> inputSet = renderer.CreateRealNumberSet<num_t>();
        std::vector<State> states = std::vector<State>();
        for (int i = 1; i < argc; i++) {
            Node* root = Tokenize(Preproces(argv[i]));
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
            states.push_back(state);
        }
        size_t state = 0;
        const auto func = [&states, &state](num_t x) {
            const Function* funcNode = states.at(state).GetFunction("f");
            State tmp = states.at(state);
            tmp.variables.push_back(Variable(funcNode->arguments[0].name, std::to_string(x)));
            Node* n = Optimize(funcNode->body, tmp);
            const std::vector<std::complex<num_t>> complexRet = n->ToNumber();
            delete n;
            std::vector<num_t> ret;
            for (const std::complex<num_t>& i : complexRet) ret.push_back(i.imag() == 0 ? i.real() : NAN);
            return ret;
        };
        const auto complexFunc = [&states, &state](std::complex<num_t> z) {
            const Function* funcNode = states.at(state).GetFunction("f");
            State tmp = states.at(state);
            tmp.variables.push_back(Variable(funcNode->arguments[0].name, ComplexToString<num_t>(z)));
            Node* n = Optimize(funcNode->body, tmp);
            const std::complex<num_t> ret = n->ToNumber().at(0);
            delete n;
            return ret;
        };
        if (!HandleEvents(renderer, [&renderer, func, complexFunc, &inputSet, &states, &state](void) {
            renderer.Fill(0x000000ff);
            renderer.DrawAxis(0xffffffff, 0x808080ff);
            const Function* funcNode = states.at(state).GetFunction("f");
            if (funcNode == nullptr) return false;
            else if (funcNode->domain == "C" && funcNode->codomain == "C")
                renderer.DrawComplexFunction<num_t>(renderer.GenerateComplexFunction<num_t>(complexFunc, inputSet));
            else if (funcNode->domain == "R" && funcNode->codomain == "R")
                renderer.DrawFunction<num_t>(renderer.GenerateFunction<num_t>(func, inputSet), 0xff0000ff);
            else return false;
            return renderer.Update();
        }, inputSet, state, states.size())) throw std::runtime_error("Failed to render function");
        for (State& state : states) {
            for (Variable& var : state.variables) delete var.value;
            for (Function& func : state.functions) delete func.body;
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}