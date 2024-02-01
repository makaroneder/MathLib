#define FillGapsInFunctions
#include "SDL2.hpp"
#include "Tokenizer.hpp"
#include "Optimizer.hpp"
#include <Log.hpp>
#include <iostream>
#include <fstream>

// TODO: Add derivatives to tokenizer and optimizer

#define CheckError(x) if (!(x)) throw std::runtime_error("Error in function: " + std::string(__func__) + ", file: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));

/// @brief Calculates value of function f(x)
/// @param node Root of the node tree containing the function
/// @param x Current value of the function argument
/// @param variables Array of variable names and their values
/// @param builtinFuncs Builtin functions
/// @return Value of function f(x)
std::string Evaluate(Node* node, std::string x, std::vector<Variable> variables, std::vector<BuiltinFunction> builtinFuncs) {
    std::vector<Function> funcs;
    delete Optimize(node, variables, funcs, builtinFuncs);
    std::string ret = "";
    for (Function& func : funcs) {
        if (func.name == "f") {
            variables.push_back(Variable("x", x));
            Node* n = Optimize(func.body, variables, funcs, builtinFuncs);
            ret = n->value;
            delete n;
            break;
        }
    }
    return ret;
}
/// @brief Reads file from given path
/// @param path Path to the file to read
/// @return Contents of the specified file
std::string ReadFile(std::string path) {
    std::ifstream file;
    file.open(path);
    std::string ret = "";
    if (file.is_open()) {
        while (file) {
            std::string line;
            std::getline(file, line);
            ret += line + "\n";
        }
    }
    return ret;
}
/// @brief Entry point for this program
/// @return Status
int main(int argc, char** argv) {
    try {
        clock_t now = clock();
        SDL2Renderer renderer = SDL2Renderer(800, 800);
        std::vector<num_t> set = renderer.CreateRealNumberSet<num_t>();
        CheckError(renderer.DrawAxis(0xffffffff));
        if (argc >= 2) {
            Node* root = Tokenize(ReadFile(argv[1]));
            std::vector<BuiltinFunction> funcs = {
                BuiltinFunction("log", [](std::vector<Node*> args) {
                    return args.size() == 2 ? new Node(Node::Type::Constant, std::to_string(Log<num_t>(std::stold(args[0]->value), std::stold(args[1]->value)))) : nullptr;
                }),
                BuiltinFunction("sin", [](std::vector<Node*> args) {
                    return args.size() == 1 ? new Node(Node::Type::Constant, std::to_string(std::sin(std::stold(args[0]->value)))) : nullptr;
                }),
                // TODO: Add more tygonometric functions
            };
            #ifdef PrintNodes
            std::cout << "Generated nodes:\n";
            root->Print();
            #endif
            auto func = [root, funcs](num_t x) {
                return std::stold(Evaluate(root, std::to_string(x), {
                    Variable("e", std::to_string(M_El)),
                    Variable("p", std::to_string(M_PIl)),
                }, funcs));
            };
            CheckError(renderer.DrawFunction<num_t>(func, 0xff0000ff, set, set));
            CheckError(renderer.DrawDerivativeFunction<num_t>(func, 0x00ff00ff, set, set));
            delete root;
        }
        else {
            auto func = [](num_t x) {
                return std::cos(x);
            };
            CheckError(renderer.DrawFunction<num_t>(func, 0xff0000ff, set, set));
            CheckError(renderer.DrawDerivativeFunction<num_t>(func, 0x00ff00ff, set, set));
        }
        CheckError(renderer.Update());
        printf("Time: %Lf second(s)\n", (num_t)(clock() - now) / CLOCKS_PER_SEC);
        renderer.Quit();
        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
}