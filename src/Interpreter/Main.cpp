#include <MathLib.hpp>
#include <iostream>

// TODO: logical and, logical or, for loops, structs, pointers

[[nodiscard]] MathLib::Node* Print(const void*, const MathLib::Array<const MathLib::Node*>& args) {
    if (args.GetSize() != 1) return nullptr;
    if (args.At(0)->type == MathLib::Node::Type::Constant) std::cout << args.At(0)->ToNumber().At(0) << std::flush;
    else if (args.At(0)->type == MathLib::Node::Type::String) std::cout << args.At(0)->value << std::flush;
    return nullptr;
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
            MathLib::BuiltinFunction("Print", MathLib::BuiltinFunctionPointer(nullptr, &Print)),
        }, std::vector<MathLib::FunctionNode> {}, std::vector<MathLib::Variable> {
            MathLib::Variable("newLine", "String", new MathLib::Node(MathLib::Node::Type::String, "\n"), true),
        });
        MathLib::Node* optimizedRoot = optimizer.Optimize(root);
        delete root;
        #ifdef Debug
        std::cout << "Optimized nodes:\n" << *optimizedRoot << std::endl;
        #endif
        delete optimizedRoot;
        optimizer.runtime = true;
        MathLib::FunctionNode main = optimizer.GetFunction("Main");
        MathLib::Array<MathLib::Node*> args = MathLib::Array<MathLib::Node*>(argc - 1);
        for (int i = 1; i < argc; i++) args.At(i - 1) = new MathLib::Node(MathLib::Node::Type::String, argv[i]);
        if (!optimizer.variables.Add(MathLib::Variable(main.arguments[0].name, main.arguments[0].dataType, MathLib::ToString(argc - 1), true)) || !optimizer.variables.Add(MathLib::Variable(main.arguments[1].name, main.arguments[1].dataType, MathLib::ArrayToComma(args), true))) MathLib::Panic("Failed to add variables for Main function");
        MathLib::Node* tmp = optimizer.Optimize(main.body);
        if (tmp->type != MathLib::Node::Type::Constant) MathLib::Panic("Invalid type of return value from Main");
        if (!tmp->ToNumber().At(0).ToReal()) MathLib::Panic("Unexpected return value from Main");
        delete tmp;
        optimizer.Destroy();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}