#define FillGapsInFunctions
#include <SDL2.hpp>
#include <Color.hpp>
#include <Physics.hpp>
#include <EquationSolver/Optimizer.hpp>
#include <EquationSolver/Tokenizer.hpp>
#include <EquationSolver/Preprocesor.hpp>
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
        const SDL_Event event = renderer.GetEvent();
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
                    if (GetVectorAxis(renderer.position, VectorAxis::X) >= (-17 + speed * baseMultiplier / renderer.pointMultiplier)) {
                        GetVectorAxis(renderer.position, VectorAxis::X) -= speed * baseMultiplier / renderer.pointMultiplier;
                        for (num_t& x : inputSet) x -= speed * baseMultiplier / renderer.pointMultiplier;
                    }
                    break;
                }
                case 'q': {
                    if (renderer.pointMultiplier > (9 + speed) && FloatsEqual<num_t>(GetVectorAxis(renderer.position, VectorAxis::X), 0, 1 / renderer.pointMultiplier) && FloatsEqual<num_t>(GetVectorAxis(renderer.position, VectorAxis::Y), 0, 1 / renderer.pointMultiplier)) {
                        renderer.pointMultiplier -= speed;
                        inputSet = renderer.CreateRealNumberSet<num_t>();
                    }
                    break;
                }
                case 'e': {
                    if (FloatsEqual<num_t>(GetVectorAxis(renderer.position, VectorAxis::X), 0, 1 / renderer.pointMultiplier) && FloatsEqual<num_t>(GetVectorAxis(renderer.position, VectorAxis::Y), 0, 1 / renderer.pointMultiplier)) {
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
        std::vector<num_t> outputSet = {};
        CheckError(renderer.Update());
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
            const std::vector<num_t> ret = n->ToNumber();
            delete n;
            return ret;
        };
        CheckError(HandleEvents(renderer, [&renderer, func, &inputSet, outputSet](void) {
            if (!renderer.DrawAxis(0xffffffff)) return false;
            if (!renderer.DrawFunction<num_t>(renderer.GenerateFunction<num_t>(func, inputSet, outputSet), 0xff0000ff)) return false;
            return renderer.Update();
        }, inputSet, state, states.size()));
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}