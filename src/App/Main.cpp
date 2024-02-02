#define FillGapsInFunctions
#include "SDL2.hpp"
#include "Tokenizer.hpp"
#include "Optimizer.hpp"
#include <Trigonometry.hpp>
#include <Log.hpp>
#include <iostream>
#include <fstream>

// TODO: Add derivatives to tokenizer and optimizer

#define CheckError(x) if (!(x)) throw std::runtime_error("Error in function: " + std::string(__func__) + ", file: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));

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
        std::vector<num_t> inputSet = renderer.CreateRealNumberSet<num_t>();
        std::vector<num_t> outputSet = {};
        CheckError(renderer.Update());
        CheckError(renderer.DrawAxis(0xffffffff));
        if (argc >= 2) {
            Node* root = Tokenize(ReadFile(argv[1]));
            std::vector<BuiltinFunction> builtinFuncs = {
                #define CreateBuiltinFunction(name, func, argc, ...)                                                            \
                BuiltinFunction(name, [](std::vector<Node*> args) {                                                             \
                    return args.size() == argc ? new Node(Node::Type::Constant, std::to_string(func(__VA_ARGS__))) : nullptr;   \
                }),
                CreateBuiltinFunction("log", Log<num_t>, 2, std::stold(args[0]->value), std::stold(args[1]->value))
                CreateBuiltinFunction("sin", std::sin, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("cos", std::cos, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("tan", std::tan, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("cot", Cot<num_t>, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("sec", Sec<num_t>, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("csc", Csc<num_t>, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("crd", Crd<num_t>, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("exsec", Exsec<num_t>, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("excsc", Excsc<num_t>, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("versin", Versin<num_t>, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("coversin", Coversin<num_t>, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("vercosin", Vercosin<num_t>, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("covercosin", Covercosin<num_t>, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("haversin", Haversin<num_t>, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("hacoversin", Hacoversin<num_t>, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("havercosin", Havercosin<num_t>, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("hacovercosin", Hacovercosin<num_t>, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("sinh", std::sinh, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("cosh", std::cosh, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("tanh", std::tanh, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("coth", HyperbolicCot<num_t>, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("sech", HyperbolicSec<num_t>, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("csch", HyperbolicCsc<num_t>, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("arcsin", std::asin, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("arccos", std::acos, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("arctan", std::atan, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("arccot", InversedCot<num_t>, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("arcsec", InversedSec<num_t>, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("arccsc", InversedCsc<num_t>, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("arcsinh", std::asinh, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("arccosh", std::acosh, 1, std::stold(args[0]->value))
                CreateBuiltinFunction("arctanh", std::atanh, 1, std::stold(args[0]->value))
            };
            #ifdef PrintNodes
            std::cout << "Generated nodes:\n";
            root->Print();
            #endif
            std::vector<Variable> variables = {
                Variable("e", std::to_string(M_El)),
                Variable("pi", std::to_string(M_PIl)),
            };
            std::vector<Function> funcs;
            Node* optimizedRoot = Optimize(root, variables, funcs, builtinFuncs);
            delete root;
            #ifdef PrintNodes
            std::cout << "Optimized nodes:\n";
            optimizedRoot->Print();
            #endif
            delete optimizedRoot;
            Function* funcNode = nullptr;
            for (Function& func : funcs) {
                if (func.name == "f") {
                    funcNode = &func;
                    break;
                }
            }
            if (funcNode == nullptr) throw std::runtime_error("No function 'f' in specified program");
            auto func = [funcNode, variables, funcs, builtinFuncs](num_t x) {
                std::vector<Variable> vars = variables;
                vars.push_back(Variable("x", std::to_string(x)));
                std::vector<Function> tmp = funcs;
                Node* n = Optimize(funcNode->body, vars, tmp, builtinFuncs);
                num_t ret = std::stold(n->value);
                delete n;
                return ret;
            };
            CheckError(renderer.DrawFunction<num_t>(func, 0xff0000ff, inputSet, outputSet));
        }
        else {
            auto func = [](num_t x) {
                return std::tan(x);
            };
            CheckError(renderer.DrawFunction<num_t>(func, 0xff0000ff, inputSet, outputSet));
            CheckError(renderer.DrawDerivativeFunction<num_t>(func, 0x00ff00ff, inputSet, outputSet));
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