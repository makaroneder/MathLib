#include <MathLib.hpp>
#include <iostream>

Node* Encrypt(const void*, const Array<const Node*>& args) {
    for (const Node* const& arg : args)
        if (arg->type != Node::Type::String) return nullptr;
    if (args.At(0)->value == "CaesarCipher") return new Node(Node::Type::String, CaesarCipher().Encrypt(args.At(1)->value, args.At(2)->value));
    else if (args.At(0)->value == "AffineCipher") return new Node(Node::Type::String, AffineCipher().Encrypt(args.At(1)->value, args.At(2)->value));
    else if (args.At(0)->value == "AtbashCipher") return new Node(Node::Type::String, AtbashCipher().Encrypt(args.At(1)->value, args.At(2)->value));
    else if (args.At(0)->value == "ROT13") return new Node(Node::Type::String, ROT13().Encrypt(args.At(1)->value, args.At(2)->value));
    else return nullptr;
}
Node* Decrypt(const void*, const Array<const Node*>& args) {
    for (const Node* const& arg : args)
        if (arg->type != Node::Type::String) return nullptr;
    if (args.At(0)->value == "CaesarCipher") return new Node(Node::Type::String, CaesarCipher().Decrypt(args.At(1)->value, args.At(2)->value));
    else if (args.At(0)->value == "AffineCipher") return new Node(Node::Type::String, AffineCipher().Decrypt(args.At(1)->value, args.At(2)->value));
    else if (args.At(0)->value == "AtbashCipher") return new Node(Node::Type::String, AtbashCipher().Decrypt(args.At(1)->value, args.At(2)->value));
    else if (args.At(0)->value == "ROT13") return new Node(Node::Type::String, ROT13().Decrypt(args.At(1)->value, args.At(2)->value));
    else return nullptr;
}
Node* Print(const void*, const Array<const Node*>& args) {
    for (const Node* const& arg : args) {
        if (arg->type != Node::Type::String) return nullptr;
        std::cout << arg->value;
    }
    std::cout << '\n';
    return new Node(Node::Type::Constant, "0");
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
        Node* root = Tokenize(Preproces(fs, argv[1]));
        #ifdef Debug
        std::cout << "Generated nodes:\n" << *root << std::endl;
        #endif
        EquationSolverState state = EquationSolverState(std::vector<BuiltinFunction> {
            BuiltinFunction("Encrypt", BuiltinFunctionPointer(nullptr, &Encrypt)),
            BuiltinFunction("Decrypt", BuiltinFunctionPointer(nullptr, &Decrypt)),
            BuiltinFunction("Print", BuiltinFunctionPointer(nullptr, &Print)),
        });
        Node* optimizedRoot = Optimize(root, state);
        delete root;
        state.Destroy();
        #ifdef Debug
        std::cout << "Optimized nodes:\n" << *optimizedRoot << std::endl;
        #endif
        delete optimizedRoot;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}