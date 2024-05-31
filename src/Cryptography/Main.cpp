#include <EquationSolver/Preprocesor.hpp>
#include <Cryptography/AtbashCipher.hpp>
#include <EquationSolver/Tokenizer.hpp>
#include <EquationSolver/Optimizer.hpp>
#include <Cryptography/ROT13.hpp>
#include <Libc/HostFileSystem.hpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2) Panic("No input files provided");
        HostFileSystem fs;
        Node* root = Tokenize(Preproces(fs, argv[1]));
        #ifdef Debug
        std::cout << "Generated nodes:\n" << *root << std::endl;
        #endif
        State state = State(std::vector<BuiltinFunction> {
            BuiltinFunction("Encrypt", [](Array<const Node*> args) -> Node* {
                for (size_t i = 0; i < args.GetSize(); i++)
                    if (args.At(i)->type != Node::Type::String) return nullptr;
                if (args.At(0)->value == "CaesarCipher") return new Node(Node::Type::String, CaesarCipher().Encrypt(args.At(1)->value, args.At(2)->value));
                else if (args.At(0)->value == "AffineCipher") return new Node(Node::Type::String, AffineCipher().Encrypt(args.At(1)->value, args.At(2)->value));
                else if (args.At(0)->value == "AtbashCipher") return new Node(Node::Type::String, AtbashCipher().Encrypt(args.At(1)->value, args.At(2)->value));
                else if (args.At(0)->value == "ROT13") return new Node(Node::Type::String, ROT13().Encrypt(args.At(1)->value, args.At(2)->value));
                else return nullptr;
            }),
            BuiltinFunction("Decrypt", [](Array<const Node*> args) -> Node* {
                for (size_t i = 0; i < args.GetSize(); i++)
                    if (args.At(i)->type != Node::Type::String) return nullptr;
                if (args.At(0)->value == "CaesarCipher") return new Node(Node::Type::String, CaesarCipher().Decrypt(args.At(1)->value, args.At(2)->value));
                else if (args.At(0)->value == "AffineCipher") return new Node(Node::Type::String, AffineCipher().Decrypt(args.At(1)->value, args.At(2)->value));
                else if (args.At(0)->value == "AtbashCipher") return new Node(Node::Type::String, AtbashCipher().Decrypt(args.At(1)->value, args.At(2)->value));
                else if (args.At(0)->value == "ROT13") return new Node(Node::Type::String, ROT13().Decrypt(args.At(1)->value, args.At(2)->value));
                else return nullptr;
            }),
            BuiltinFunction("Print", [](Array<const Node*> args) -> Node* {
                for (size_t i = 0; i < args.GetSize(); i++) {
                    if (args.At(i)->type != Node::Type::String) return nullptr;
                    std::cout << args.At(i)->value;
                }
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