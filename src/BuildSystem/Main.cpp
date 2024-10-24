#include "Target.hpp"
#include "FileSearch.hpp"
#include "FileTranslation.hpp"
#ifdef IncludeSources
#include "Target.cpp"
#include "FileSearch.cpp"
#include "FileTranslation.cpp"
#endif
#include <MathLib.hpp>
#include <iostream>

HostFileSystem fileSystem;
Array<Target> targets;
Array<FileSearch> fileSearchs;
Array<FileTranslation> fileTranslations;
String nonPhonyTargets = "";

Node* DeclareTarget(const void*, const Array<const Node*>& args) {
    if (args.GetSize() != 4) return nullptr;
    for (size_t i = 0; i < 3; i++)
        if (args.At(i)->type != Node::Type::String) return nullptr;
    if (args.At(3)->type != Node::Type::Constant) return nullptr;
    const Target target = Target(args.At(0)->value, args.At(1)->value, args.At(2)->value, args.At(3)->ToNumber().At(0).GetReal());
    if (!target.phony && Find(target.name, "%") == SIZE_MAX) nonPhonyTargets += target.name + ' ';
    return targets.Add(target) ? new Node(Node::Type::String, "") : nullptr;
}
Node* CreateObjects(const void*, const Array<const Node*>& args) {
    if (args.GetSize() != 3) return nullptr;
    for (size_t i = 0; i < args.GetSize(); i++)
        if (args.At(i)->type != Node::Type::String) return nullptr;
    return fileTranslations.Add(FileTranslation(args.At(0)->value, args.At(1)->value, args.At(2)->value)) ? new Node(Node::Type::String, String("$(FILETRANSLATION") + ToString(fileTranslations.GetSize()) + ") ") : nullptr;
}
Node* FindFiles(const void*, const Array<const Node*>& args) {
    if (args.GetSize() != 2) return nullptr;
    for (size_t i = 0; i < args.GetSize(); i++)
        if (args.At(i)->type != Node::Type::String) return nullptr;
    return fileSearchs.Add(FileSearch(args.At(0)->value, args.At(1)->value)) ? new Node(Node::Type::String, String("$(FILESEARCH") + ToString(fileSearchs.GetSize()) + ") ") : nullptr;
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
        if (argc < 3) Panic(String("Usage: ") + argv[0] + " <input file> <output file>");
        Node* root = Tokenize(Preproces(fileSystem, argv[1]));
        #ifdef Debug
        std::cout << "Generated nodes:\n" << *root << std::endl;
        #endif
        Optimizer optimizer = Optimizer(std::vector<BuiltinFunction> {
            BuiltinFunction("DeclareTarget", BuiltinFunctionPointer(nullptr, &DeclareTarget)),
            BuiltinFunction("CreateObjects", BuiltinFunctionPointer(nullptr, &CreateObjects)),
            BuiltinFunction("FindFiles", BuiltinFunctionPointer(nullptr, &FindFiles)),
        }, std::vector<FunctionNode> {}, std::vector<Variable> {
            Variable("true", "Bool", "1", true),
            Variable("false", "Bool", "0", true),
        });
        optimizer.runtime = true;
        Node* optimizedRoot = optimizer.Optimize(root);
        delete root;
        #ifdef Debug
        std::cout << "Optimized nodes:\n" << *optimizedRoot << std::endl;
        #endif
        delete optimizedRoot;
        optimizer.Destroy();
        File output = fileSystem.Open(argv[2], OpenMode::Write);
        for (size_t i = 0; i < fileSearchs.GetSize(); i++)
            output.Puts(String("FILESEARCH") + ToString(i + 1) + " = $(shell find " + fileSearchs.At(i).directory + " -type f -name \"*" + fileSearchs.At(i).extension + "\")\n");
        for (size_t i = 0; i < fileTranslations.GetSize(); i++)
            output.Puts(String("FILETRANSLATION") + ToString(i + 1) + " = $(patsubst " + fileTranslations.At(i).outputFormat + ", " + fileTranslations.At(i).inputFormat + ", " + fileTranslations.At(i).sources + ")\n");
        output.Puts(String("all: ") + nonPhonyTargets + "\n.PHONY: all\n");
        for (const Target& target : targets) {
            #ifdef Debug
            std::cout << target << std::endl;
            #endif
            output.Puts(target.name + ": " + target.deps + '\n');
            if (!target.phony) output.Puts("\t@mkdir -p $(@D)\n");
            output.Puts(String("\t@") + target.command + '\n');
            if (!target.phony) output.Puts("\t@echo \"==> Created: $@\"\n");
            else output.Puts(String(".PHONY: ") + target.name + '\n');
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}