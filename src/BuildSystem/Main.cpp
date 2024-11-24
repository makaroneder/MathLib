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

MathLib::HostFileSystem fileSystem;
MathLib::Array<Target> targets;
MathLib::Array<FileSearch> fileSearchs;
MathLib::Array<FileTranslation> fileTranslations;
MathLib::String nonPhonyTargets = "";

MathLib::Node* DeclareTarget(const void*, const MathLib::Array<const MathLib::Node*>& args) {
    if (args.GetSize() != 4) return nullptr;
    for (size_t i = 0; i < 3; i++)
        if (args.At(i)->type != MathLib::Node::Type::String) return nullptr;
    if (args.At(3)->type != MathLib::Node::Type::Constant) return nullptr;
    const Target target = Target(args.At(0)->value, args.At(1)->value, args.At(2)->value, args.At(3)->ToNumber().At(0).GetReal());
    if (!target.phony && Find(target.name, "%") == SIZE_MAX) nonPhonyTargets += target.name + ' ';
    return targets.Add(target) ? new MathLib::Node(MathLib::Node::Type::String, "") : nullptr;
}
MathLib::Node* CreateObjects(const void*, const MathLib::Array<const MathLib::Node*>& args) {
    if (args.GetSize() != 3) return nullptr;
    for (size_t i = 0; i < args.GetSize(); i++)
        if (args.At(i)->type != MathLib::Node::Type::String) return nullptr;
    return fileTranslations.Add(FileTranslation(args.At(0)->value, args.At(1)->value, args.At(2)->value)) ? new MathLib::Node(MathLib::Node::Type::String, MathLib::String("$(FILETRANSLATION") + MathLib::ToString(fileTranslations.GetSize()) + ") ") : nullptr;
}
MathLib::Node* FindFiles(const void*, const MathLib::Array<const MathLib::Node*>& args) {
    if (args.GetSize() != 2) return nullptr;
    for (size_t i = 0; i < args.GetSize(); i++)
        if (args.At(i)->type != MathLib::Node::Type::String) return nullptr;
    return fileSearchs.Add(FileSearch(args.At(0)->value, args.At(1)->value)) ? new MathLib::Node(MathLib::Node::Type::String, MathLib::String("$(FILESEARCH") + MathLib::ToString(fileSearchs.GetSize()) + ") ") : nullptr;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 3) MathLib::Panic(MathLib::String("Usage: ") + argv[0] + " <input file> <output file>");
        MathLib::Node* root = MathLib::Tokenize(MathLib::Preproces(fileSystem, argv[1]));
        #ifdef Debug
        std::cout << "Generated nodes:\n" << *root << std::endl;
        #endif
        MathLib::Optimizer optimizer = MathLib::Optimizer(std::vector<MathLib::BuiltinFunction> {
            MathLib::BuiltinFunction("DeclareTarget", MathLib::BuiltinFunctionPointer(nullptr, &DeclareTarget)),
            MathLib::BuiltinFunction("CreateObjects", MathLib::BuiltinFunctionPointer(nullptr, &CreateObjects)),
            MathLib::BuiltinFunction("FindFiles", MathLib::BuiltinFunctionPointer(nullptr, &FindFiles)),
        }, std::vector<MathLib::FunctionNode> {}, std::vector<MathLib::Variable> {});
        optimizer.runtime = true;
        MathLib::Node* optimizedRoot = optimizer.Optimize(root);
        delete root;
        #ifdef Debug
        std::cout << "Optimized nodes:\n" << *optimizedRoot << std::endl;
        #endif
        delete optimizedRoot;
        optimizer.Destroy();
        MathLib::File output = fileSystem.Open(argv[2], MathLib::OpenMode::Write);
        for (size_t i = 0; i < fileSearchs.GetSize(); i++)
            output.Puts(MathLib::String("FILESEARCH") + MathLib::ToString(i + 1) + " = $(shell find " + fileSearchs.At(i).directory + " -type f -name \"*" + fileSearchs.At(i).extension + "\")\n");
        for (size_t i = 0; i < fileTranslations.GetSize(); i++)
            output.Puts(MathLib::String("FILETRANSLATION") + MathLib::ToString(i + 1) + " = $(patsubst " + fileTranslations.At(i).outputFormat + ", " + fileTranslations.At(i).inputFormat + ", " + fileTranslations.At(i).sources + ")\n");
        output.Puts(MathLib::String("all: ") + nonPhonyTargets + "\n.PHONY: all\n");
        for (const Target& target : targets) {
            #ifdef Debug
            std::cout << target << std::endl;
            #endif
            output.Puts(target.name + ": " + target.deps + '\n');
            if (!target.phony) output.Puts("\t@mkdir -p $(@D)\n");
            output.Puts(MathLib::String("\t@") + target.command + '\n');
            if (!target.phony) output.Puts("\t@echo \"==> Created: $@\"\n");
            else output.Puts(MathLib::String(".PHONY: ") + target.name + '\n');
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}