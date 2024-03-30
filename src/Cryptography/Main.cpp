#include <EquationSolver/Preprocesor.hpp>
#include <Cryptography/AtbashCipher.hpp>
#include <EquationSolver/Tokenizer.hpp>
#include <EquationSolver/Optimizer.hpp>
#include <Cryptography/ROT13.hpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2) throw std::runtime_error("No input files provided");
        Node* root = Tokenize(Preproces(argv[1]));
        #ifdef Debug
        std::cout << "Generated nodes:\n" << *root << std::endl;
        #endif
        State state = State({
            BuiltinFunction("Encrypt", [](std::vector<const Node*> args) -> Node* {
                for (const Node* node : args)
                    if (node->type != Node::Type::String) return nullptr;
                if (args.at(0)->value == "CaesarCipher") return new Node(Node::Type::String, CaesarCipher().Encrypt(args.at(1)->value, args.at(2)->value));
                else if (args.at(0)->value == "AffineCipher") return new Node(Node::Type::String, AffineCipher().Encrypt(args.at(1)->value, args.at(2)->value));
                else if (args.at(0)->value == "AtbashCipher") return new Node(Node::Type::String, AtbashCipher().Encrypt(args.at(1)->value, args.at(2)->value));
                else if (args.at(0)->value == "ROT13") return new Node(Node::Type::String, ROT13().Encrypt(args.at(1)->value, args.at(2)->value));
                else return nullptr;
            }),
            BuiltinFunction("Decrypt", [](std::vector<const Node*> args) -> Node* {
                for (const Node* node : args)
                    if (node->type != Node::Type::String) return nullptr;
                if (args.at(0)->value == "CaesarCipher") return new Node(Node::Type::String, CaesarCipher().Decrypt(args.at(1)->value, args.at(2)->value));
                else if (args.at(0)->value == "AffineCipher") return new Node(Node::Type::String, AffineCipher().Decrypt(args.at(1)->value, args.at(2)->value));
                else if (args.at(0)->value == "AtbashCipher") return new Node(Node::Type::String, AtbashCipher().Decrypt(args.at(1)->value, args.at(2)->value));
                else if (args.at(0)->value == "ROT13") return new Node(Node::Type::String, ROT13().Decrypt(args.at(1)->value, args.at(2)->value));
                else return nullptr;
            }),
            BuiltinFunction("Print", [](std::vector<const Node*> args) -> Node* {
                for (const Node* node : args)
                    if (node->type != Node::Type::String) return nullptr;
                for (const Node* node : args) std::cout << node->value;
                std::cout << '\n';
                return new Node(Node::Type::Constant, "0");
            }),
        });
        Node* optimizedRoot = Optimize(root, state);
        #ifdef Debug
        std::cout << "Optimized nodes:\n" << *optimizedRoot << std::endl;
        #endif
        delete optimizedRoot;
        delete root;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}