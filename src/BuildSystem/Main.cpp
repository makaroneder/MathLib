#include "Target.hpp"
#include "FileSearch.hpp"
#include "FileTranslation.hpp"
#include <EquationSolver/Preprocesor.hpp>
#include <EquationSolver/Tokenizer.hpp>
#include <EquationSolver/Optimizer.hpp>
#include <Libc/HostFileSystem.hpp>
#include <iostream>

MathLib::HostFileSystem fileSystem;
MathLib::Array<Target> targets;
MathLib::Array<FileSearch> fileSearchs;
MathLib::Array<FileTranslation> fileTranslations;
MathLib::String nonPhonyTargets = "";

[[nodiscard]] MathLib::Node* DeclareTarget(const void*, const MathLib::Sequence<const MathLib::Node*>& args) {
    if (args.GetSize() != 4) return nullptr;
    for (size_t i = 0; i < 3; i++)
        if (args.At(i)->type != MathLib::Node::Type::String) return nullptr;
    if (args.At(3)->type != MathLib::Node::Type::Constant) return nullptr;
    const Target target = Target(args.At(0)->value, args.At(1)->value, args.At(2)->value, args.At(3)->ToNumber().At(0).GetReal());
    if (!target.phony && !target.name.Contains('%')) nonPhonyTargets += target.name + ' ';
    return targets.Add(target) ? new MathLib::Node(MathLib::Node::Type::String, ""_M) : nullptr;
}
[[nodiscard]] MathLib::Node* CreateObjects(const void*, const MathLib::Sequence<const MathLib::Node*>& args) {
    if (args.GetSize() != 3) return nullptr;
    for (size_t i = 0; i < args.GetSize(); i++)
        if (args.At(i)->type != MathLib::Node::Type::String) return nullptr;
    return fileTranslations.Add(FileTranslation(args.At(0)->value, args.At(1)->value, args.At(2)->value)) ? new MathLib::Node(MathLib::Node::Type::String, "$(FILETRANSLATION"_M + MathLib::ToString(fileTranslations.GetSize()) + ") ") : nullptr;
}
[[nodiscard]] MathLib::Node* FindFiles(const void*, const MathLib::Sequence<const MathLib::Node*>& args) {
    if (args.GetSize() != 2) return nullptr;
    for (size_t i = 0; i < args.GetSize(); i++)
        if (args.At(i)->type != MathLib::Node::Type::String) return nullptr;
    return fileSearchs.Add(FileSearch(args.At(0)->value, args.At(1)->value)) ? new MathLib::Node(MathLib::Node::Type::String, "$(FILESEARCH"_M + MathLib::ToString(fileSearchs.GetSize()) + ") ") : nullptr;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 3) MathLib::Panic("Usage: "_M + argv[0] + " <input file> <output file>");
        MathLib::Node* root = MathLib::Tokenize(MathLib::Preproces(fileSystem, MathLib::String(argv[1])));
        #ifdef Debug
        std::cout << "Generated nodes:\n" << *root << std::endl;
        #endif
        MathLib::Optimizer optimizer = MathLib::Optimizer(MathLib::MakeArray<MathLib::BuiltinFunction>(
            MathLib::BuiltinFunction("DeclareTarget"_M, MathLib::BuiltinFunctionPointer(nullptr, &DeclareTarget)),
            MathLib::BuiltinFunction("CreateObjects"_M, MathLib::BuiltinFunctionPointer(nullptr, &CreateObjects)),
            MathLib::BuiltinFunction("FindFiles"_M, MathLib::BuiltinFunctionPointer(nullptr, &FindFiles))
        ));
        optimizer.runtime = true;
        MathLib::Node* optimizedRoot = optimizer.Optimize(root);
        delete root;
        #ifdef Debug
        std::cout << "Optimized nodes:\n" << *optimizedRoot << std::endl;
        #endif
        delete optimizedRoot;
        optimizer.Destroy();
        MathLib::File output = fileSystem.Open(MathLib::String(argv[2]), MathLib::OpenMode::Write);
        for (size_t i = 0; i < fileSearchs.GetSize(); i++)
            if (!output.Puts("FILESEARCH"_M + MathLib::ToString(i + 1) + " = $(call rwildcard," + fileSearchs.At(i).directory + ",*" + fileSearchs.At(i).extension + ")\n")) MathLib::Panic("Failed to write output data");
        for (size_t i = 0; i < fileTranslations.GetSize(); i++)
            if (!output.Puts("FILETRANSLATION"_M + MathLib::ToString(i + 1) + " = $(patsubst " + fileTranslations.At(i).outputFormat + ", " + fileTranslations.At(i).inputFormat + ", " + fileTranslations.At(i).sources + ")\n")) MathLib::Panic("Failed to write output data");
        if (!output.Puts("all: "_M + nonPhonyTargets + "\n.PHONY: all\n")) MathLib::Panic("Failed to write output data");
        for (const Target& target : targets) {
            #ifdef Debug
            std::cout << target << std::endl;
            #endif
            if (!output.Puts(target.name + ": " + target.deps + '\n') || (!target.phony && !output.Puts("\t@$(MKDIR) $(@D)\n")) || !output.Puts("\t@"_M + target.command + '\n')) MathLib::Panic("Failed to write output data");
            if (!target.phony) {
                if (!output.Puts("\t@echo \"==> Created: $@\"\n")) MathLib::Panic("Failed to write output data");
            }
            else if (!output.Puts(".PHONY: "_M + target.name + '\n')) MathLib::Panic("Failed to write output data");
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}