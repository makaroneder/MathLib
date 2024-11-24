#include <MathLib.hpp>
#include <iostream>

MathLib::Node* Encrypt(const void*, const MathLib::Array<const MathLib::Node*>& args) {
    for (const MathLib::Node* const& arg : args)
        if (arg->type != MathLib::Node::Type::String) return nullptr;
    if (args.At(0)->value == "CaesarCipher") return new MathLib::Node(MathLib::Node::Type::String, MathLib::CaesarCipher().EncryptString(args.At(1)->value, args.At(2)->value));
    else if (args.At(0)->value == "AffineCipher") return new MathLib::Node(MathLib::Node::Type::String, MathLib::AffineCipher().EncryptString(args.At(1)->value, args.At(2)->value));
    else if (args.At(0)->value == "AtbashCipher") return new MathLib::Node(MathLib::Node::Type::String, MathLib::AtbashCipher().EncryptString(args.At(1)->value, args.At(2)->value));
    else if (args.At(0)->value == "ROT13") return new MathLib::Node(MathLib::Node::Type::String, MathLib::ROT13().EncryptString(args.At(1)->value, args.At(2)->value));
    else return nullptr;
}
MathLib::Node* Decrypt(const void*, const MathLib::Array<const MathLib::Node*>& args) {
    for (const MathLib::Node* const& arg : args)
        if (arg->type != MathLib::Node::Type::String) return nullptr;
    if (args.At(0)->value == "CaesarCipher") return new MathLib::Node(MathLib::Node::Type::String, MathLib::CaesarCipher().DecryptString(args.At(1)->value, args.At(2)->value));
    else if (args.At(0)->value == "AffineCipher") return new MathLib::Node(MathLib::Node::Type::String, MathLib::AffineCipher().DecryptString(args.At(1)->value, args.At(2)->value));
    else if (args.At(0)->value == "AtbashCipher") return new MathLib::Node(MathLib::Node::Type::String, MathLib::AtbashCipher().DecryptString(args.At(1)->value, args.At(2)->value));
    else if (args.At(0)->value == "ROT13") return new MathLib::Node(MathLib::Node::Type::String, MathLib::ROT13().DecryptString(args.At(1)->value, args.At(2)->value));
    else return nullptr;
}
MathLib::Node* Print(const void*, const MathLib::Array<const MathLib::Node*>& args) {
    for (const MathLib::Node* const& arg : args) {
        if (arg->type != MathLib::Node::Type::String) return nullptr;
        std::cout << arg->value;
    }
    std::cout << '\n';
    return new MathLib::Node(MathLib::Node::Type::Constant, "0");
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2) MathLib::Panic(MathLib::String("Usage: ") + argv[0] + " <input file>");
        MathLib::HostFileSystem fs;
        MathLib::Node* root = MathLib::Tokenize(MathLib::Preproces(fs, argv[1]));
        #ifdef Debug
        std::cout << "Generated nodes:\n" << *root << std::endl;
        #endif
        MathLib::Optimizer optimizer = MathLib::Optimizer(std::vector<MathLib::BuiltinFunction> {
            MathLib::BuiltinFunction("Encrypt", MathLib::BuiltinFunctionPointer(nullptr, &Encrypt)),
            MathLib::BuiltinFunction("Decrypt", MathLib::BuiltinFunctionPointer(nullptr, &Decrypt)),
            MathLib::BuiltinFunction("Print", MathLib::BuiltinFunctionPointer(nullptr, &Print)),
        });
        MathLib::Node* optimizedRoot = optimizer.Optimize(root);
        delete root;
        optimizer.Destroy();
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