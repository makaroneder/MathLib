#include <Cryptography/Cipher/AtbashCipher.hpp>
#include <EquationSolver/Preprocesor.hpp>
#include <Cryptography/Cipher/ROT13.hpp>
#include <EquationSolver/Tokenizer.hpp>
#include <EquationSolver/Optimizer.hpp>
#include <Libc/HostFileSystem.hpp>
#include <FunctionT.hpp>
#include <iostream>

[[nodiscard]] MathLib::Node* Encrypt(const MathLib::Sequence<const MathLib::Node*>& args) {
    if (!args.Foreach<bool>(MathLib::MakeFunctionT<bool, bool, const MathLib::Node*>([](bool prev, const MathLib::Node* arg) -> bool {
        return prev && arg->type == MathLib::Node::Type::String;
    }), true)) return nullptr;
    if (args.At(0)->value == "CaesarCipher") return new MathLib::Node(MathLib::Node::Type::String, MathLib::CaesarCipher().EncryptString(args.At(1)->value, MathLib::Cipher::StringToKey(args.At(2)->value), true));
    else if (args.At(0)->value == "AffineCipher") return new MathLib::Node(MathLib::Node::Type::String, MathLib::AffineCipher().EncryptString(args.At(1)->value, MathLib::Cipher::StringToKey(args.At(2)->value), true));
    else if (args.At(0)->value == "AtbashCipher") return new MathLib::Node(MathLib::Node::Type::String, MathLib::AtbashCipher().EncryptString(args.At(1)->value, MathLib::Cipher::StringToKey(args.At(2)->value), true));
    else if (args.At(0)->value == "ROT13") return new MathLib::Node(MathLib::Node::Type::String, MathLib::ROT13().EncryptString(args.At(1)->value, MathLib::Cipher::StringToKey(args.At(2)->value), true));
    else return nullptr;
}
[[nodiscard]] MathLib::Node* Decrypt(const MathLib::Sequence<const MathLib::Node*>& args) {
    if (!args.Foreach<bool>(MathLib::MakeFunctionT<bool, bool, const MathLib::Node*>([](bool prev, const MathLib::Node* arg) -> bool {
        return prev && arg->type == MathLib::Node::Type::String;
    }), true)) return nullptr;
    if (args.At(0)->value == "CaesarCipher") return new MathLib::Node(MathLib::Node::Type::String, MathLib::CaesarCipher().EncryptString(args.At(1)->value, MathLib::Cipher::StringToKey(args.At(2)->value), false));
    else if (args.At(0)->value == "AffineCipher") return new MathLib::Node(MathLib::Node::Type::String, MathLib::AffineCipher().EncryptString(args.At(1)->value, MathLib::Cipher::StringToKey(args.At(2)->value), false));
    else if (args.At(0)->value == "AtbashCipher") return new MathLib::Node(MathLib::Node::Type::String, MathLib::AtbashCipher().EncryptString(args.At(1)->value, MathLib::Cipher::StringToKey(args.At(2)->value), false));
    else if (args.At(0)->value == "ROT13") return new MathLib::Node(MathLib::Node::Type::String, MathLib::ROT13().EncryptString(args.At(1)->value, MathLib::Cipher::StringToKey(args.At(2)->value), false));
    else return nullptr;
}
[[nodiscard]] MathLib::Node* Print(const MathLib::Sequence<const MathLib::Node*>& args) {
    if (!args.Foreach<bool>(MathLib::MakeFunctionT<bool, bool, const MathLib::Node*>([](bool prev, const MathLib::Node* arg) -> bool {
        std::cout << arg->value;
        return prev && arg->type == MathLib::Node::Type::String;
    }), true)) return nullptr;
    std::cout << '\n';
    return new MathLib::Node(MathLib::Node::Type::Constant, '0'_M);
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <input file>");
        MathLib::HostFileSystem fs;
        MathLib::Node* root = MathLib::Tokenize(MathLib::Preproces(fs, MathLib::String(argv[1])));
        #ifdef Debug
        std::cout << "Generated nodes:\n" << *root << std::endl;
        #endif
        MathLib::Optimizer optimizer = MathLib::Optimizer(MathLib::MakeArray<MathLib::BuiltinFunction>(
            MathLib::BuiltinFunction("Encrypt"_M, MathLib::BuiltinFunctionPointer(&Encrypt)),
            MathLib::BuiltinFunction("Decrypt"_M, MathLib::BuiltinFunctionPointer(&Decrypt)),
            MathLib::BuiltinFunction("Print"_M, MathLib::BuiltinFunctionPointer(&Print))
        ));
        const MathLib::num_t start = MathLib::GetTime();
        MathLib::Node* optimizedRoot = optimizer.Optimize(root);
        delete root;
        optimizer.Destroy();
        #ifdef Debug
        std::cout << "Optimized nodes:\n" << *optimizedRoot << std::endl;
        #endif
        delete optimizedRoot;
        std::cout << "Time: " << MathLib::GetTime() - start << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}