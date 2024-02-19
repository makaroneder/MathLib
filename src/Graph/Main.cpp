#define FillGapsInFunctions
#include "SDL2.hpp"
#include <EquationSolver/Optimizer.hpp>
#include <EquationSolver/Tokenizer.hpp>
#include <EquationSolver/Preprocesor.hpp>
#include <Trigonometry.hpp>
#include <Log.hpp>
#include <iostream>
#define CheckError(x) if (!(x)) throw std::runtime_error("Error in function: " + std::string(__func__) + ", file: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));

/// @brief Main loop of this program
/// @param renderer Renderer we are using
/// @param func Function which will be called when the user sends input
/// @param inputSet Set of every input value we allow
/// @param state Current state of the program
/// @param lastState Last state of the program
/// @return Status
bool HandleEvents(SDL2Renderer& renderer, std::function<bool(void)> func, std::vector<num_t>& inputSet, size_t& state, size_t lastState) {
    bool running = true;
    const num_t speed = 1;
    const num_t baseMultiplier = renderer.pointMultiplier;
    if (!func()) return false;
    while (running) {
        SDL_Event event = renderer.GetEvent();
        if (event.type == SDL_QUIT) running = false;
        else if (event.type == SDL_KEYDOWN) {
            bool update = true;
            switch (event.key.keysym.sym) {
                case 'w': {
                    GetVectorAxis(renderer.position, VectorAxis::Y) += speed * baseMultiplier / renderer.pointMultiplier;
                    break;
                }
                case 's': {
                    GetVectorAxis(renderer.position, VectorAxis::Y) -= speed * baseMultiplier / renderer.pointMultiplier;
                    break;
                }
                case 'd': {
                    GetVectorAxis(renderer.position, VectorAxis::X) += speed * baseMultiplier / renderer.pointMultiplier;
                    for (num_t& x : inputSet) x += speed * baseMultiplier / renderer.pointMultiplier;
                    break;
                }
                case 'a': {
                    GetVectorAxis(renderer.position, VectorAxis::X) -= speed * baseMultiplier / renderer.pointMultiplier;
                    for (num_t& x : inputSet) x -= speed * baseMultiplier / renderer.pointMultiplier;
                    break;
                }
                case 'q': {
                    if (renderer.pointMultiplier > speed) {
                        renderer.pointMultiplier -= speed;
                        inputSet = renderer.CreateRealNumberSet<num_t>();
                    }
                    break;
                }
                case 'e': {
                    renderer.pointMultiplier += speed;
                    inputSet = renderer.CreateRealNumberSet<num_t>();
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
        SDL2Renderer renderer = SDL2Renderer(800, 800);
        std::vector<num_t> inputSet = renderer.CreateRealNumberSet<num_t>();
        std::vector<num_t> outputSet = {};
        CheckError(renderer.Update());
        if (argc >= 2) {
            std::vector<State> states = std::vector<State>();
            for (int i = 1; i < argc; i++) {
                Node* root = Tokenize(Preproces(argv[i]));
                #ifdef Debug
                std::cout << "Generated nodes:\n" << root->ToString() << std::endl;
                #endif
                State state = State();
                Node* optimizedRoot = Optimize(root, state);
                delete root;
                #ifdef Debug
                std::cout << "Optimized nodes:\n" << optimizedRoot->ToString() << std::endl;
                #endif
                delete optimizedRoot;
                states.push_back(state);
            }
            size_t state = 0;
            const auto func = [&states, &state](num_t x) {
                Function* funcNode = nullptr;
                for (Function& func : states.at(state).functions) {
                    if (func.name == "f") {
                        funcNode = &func;
                        break;
                    }
                }
                if (funcNode == nullptr) throw std::runtime_error("Function 'f' not found");
                if (funcNode->arguments.size() != 1) throw std::runtime_error("Invalid amount of arguments for function 'f'");
                State tmp = states.at(state);
                tmp.variables.push_back(Variable(funcNode->arguments[0].name, std::to_string(x)));
                Node* n = Optimize(funcNode->body, tmp);
                const num_t ret = std::stold(n->value);
                delete n;
                return std::complex<num_t>(ret, 0);
            };
            CheckError(HandleEvents(renderer, [&renderer, func, &inputSet, outputSet](void) {
                if (!renderer.DrawAxis(0xffffffff)) return false;
                if (!renderer.DrawFunction<num_t>(renderer.GenerateFunction<num_t>(func, inputSet, outputSet), 0xff0000ff)) return false;
                return renderer.Update();
            }, inputSet, state, states.size()));
        }
        else {
            const auto func = [](num_t x) {
                return std::complex<num_t>(std::sin(x), 0);
            };
            size_t state = 0;
            CheckError(HandleEvents(renderer, [&renderer, func, &inputSet, outputSet](void) {
                if (!renderer.DrawAxis(0xffffffff)) return false;
                if (!renderer.DrawFunction<num_t>(renderer.GenerateFunction<num_t>(func, inputSet, outputSet), 0xff0000ff)) return false;
                if (!renderer.DrawFunction<num_t>(renderer.GenerateDerivativeFunction<num_t>(func, inputSet, outputSet), 0x00ff00ff)) return false;
                const num_t h = 1 / renderer.pointMultiplier;
                const auto der = DerivativeFunction(num_t, func, h);
                if (!renderer.DrawFunction<num_t>(renderer.GenerateDerivativeFunction<num_t>(der, inputSet, outputSet), 0x0000ffff)) return false;
                if (!renderer.DrawFunction<num_t>(renderer.GenerateDerivativeFunction<num_t>(DerivativeFunction(num_t, der, h), inputSet, outputSet), 0xffff00ff)) return false;
                return renderer.Update();
            }, inputSet, state, 1));
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}