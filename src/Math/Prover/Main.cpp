#include <Compiler/Parser/LeftBinaryParserLayer.hpp>
#include <Compiler/Parser/UnwrapperParserLayer.hpp>
#include <Compiler/Parser/IdentityParserLayer.hpp>
#include <Compiler/Parser/FunctionParserLayer.hpp>
#include <Compiler/Lexer/IdentifierLexerRule.hpp>
#include <Compiler/Lexer/SingleCharLexerRule.hpp>
#include <Compiler/Lexer/WhitespaceLexerRule.hpp>
#include <Compiler/Parser/KeywordParserLayer.hpp>
#include <Compiler/Lexer/StringLexerRule.hpp>
#include <Compiler/IdentityEvaluator.hpp>
#include <Libc/HostFileSystem.hpp>
#include <Compiler/Toolchain.hpp>
#include <FunctionT.hpp>
#include <iostream>

enum class TokenType : uint8_t {
    None,
    ParenthesesStart,
    ParenthesesEnd,
    Comma,
    Bind,
    Implication,
    Equality,
    Identifier,
    Function,
};
MathLib::String NodeToString(const MathLib::ParserNode& node, bool root = true) {
    switch ((TokenType)node.GetType()) {
        case TokenType::Comma: {
            const MathLib::String ret = NodeToString(node.At(0), true) + node.GetData() + ' ' + NodeToString(node.At(1), true);
            return root ? ret : '('_M + ret + ')';
        }
        case TokenType::Bind:
        case TokenType::Implication: {
            const MathLib::String ret = NodeToString(node.At(0), true) + ' ' + node.GetData() + ' ' + NodeToString(node.At(1), true);
            return root ? ret : '('_M + ret + ')';
        }
        case TokenType::Equality: {
            const MathLib::String ret = NodeToString(node.At(0), node.At(0).GetType() != (size_t)TokenType::Implication) + ' ' + node.GetData() + ' ' + NodeToString(node.At(1), node.At(1).GetType() != (size_t)TokenType::Implication);
            return root ? ret : '('_M + ret + ')';
        }
        case TokenType::Identifier: return node.GetData();
        case TokenType::Function: return node.GetData() + '(' + NodeToString(node.At(0)) + ')';
        default: return "";
    }
}
void Substitute(MathLib::ParserNode& value, const MathLib::ParserNode& name, const MathLib::ParserNode& argument) {
    if (argument.GetType() == (size_t)TokenType::Identifier) value.ReplaceValue(name.GetData(), argument.GetData() + ':');
    else {
        MathLib::ParserNode tmp = argument;
        tmp.ModifyValue(MathLib::MakeFunctionT<MathLib::String, const MathLib::Sequence<char>&>([](const MathLib::Sequence<char>& name) -> MathLib::String {
            return MathLib::CollectionToString(name) + ':';
        }));
        value.Replace(name, tmp);
    }
}
MathLib::ParserNode InvokeFunction(const MathLib::ParserNode& function, const MathLib::Sequence<MathLib::ParserNode>& bindings) {
    if (function.GetData() == "prove" || function.GetData() == "rewrite" || function.GetData() == "splitEquality" || function.GetData() == "splitFunction") return function;
    const size_t index = bindings.Find(MathLib::MakeFunctionT<bool, MathLib::ParserNode>([&function](MathLib::ParserNode node) -> bool {
        return node.At(0).GetData() == function.GetData();
    }));
    if (index == SIZE_MAX) return MathLib::ParserNode();
    MathLib::ParserNode definition = bindings.At(index);
    MathLib::ParserNode functionArgument = function.At(0);
    MathLib::ParserNode definitionArgument = definition.At(0).At(0);
    while (true) {
        if (functionArgument.GetType() == (size_t)TokenType::Comma) {
            if (definitionArgument.GetType() != functionArgument.GetType()) return MathLib::ParserNode();
            Substitute(definition.At(1), definitionArgument.At(1), functionArgument.At(1));
            MathLib::ParserNode tmp = functionArgument.At(0);
            functionArgument = tmp;
            tmp = definitionArgument.At(0);
            definitionArgument = tmp;
        }
        else {
            Substitute(definition.At(1), definitionArgument, functionArgument);
            definition.At(1).ModifyValue(MathLib::MakeFunctionT<MathLib::String, const MathLib::Sequence<char>&>([](const MathLib::Sequence<char>& name) -> MathLib::String {
                if (name.At(name.GetSize() - 1) == ':') return MathLib::SubString(name, 0, name.GetSize() - 1);
                return MathLib::CollectionToString(name);
            }));
            return definition.At(1);
        }
    }
}
bool CheckProof(MathLib::ParserNode& theorem, const MathLib::ParserNode& proof, const MathLib::Sequence<MathLib::ParserNode>& bindings) {
    switch ((TokenType)proof.GetType()) {
        case TokenType::Comma: {
            for (uint8_t i = 0; i < 2; i++) {
                if (!CheckProof(theorem, proof.At(i), bindings)) {
                    std::cout << "Failed to apply " << NodeToString(proof.At(i)) << " to theorem " << NodeToString(theorem) << std::endl;
                    return false;
                }
            }
            return true;
        }
        case TokenType::Identifier: return true;
        case TokenType::Function: {
            if (proof.GetData() == "prove") {
                if (proof.At(0).At(0) != theorem) {
                    std::cout << "Failed to apply rule " << NodeToString(proof.At(0).At(0)) << " to theorem " << NodeToString(theorem) << std::endl;
                    return false;
                }
                return true;
            }
            if (proof.GetData() == "splitEquality") {
                if (theorem.GetType() != (size_t)TokenType::Equality && theorem.GetType() != (size_t)TokenType::Implication) {
                    std::cout << "Theorem " << NodeToString(theorem) << " should be an equality or implication" << std::endl;
                    return false;
                }
                for (uint8_t i = 0; i < 2; i++) {
                    if (!CheckProof(theorem.At(i), proof.At(0).At(i), bindings)) {
                        std::cout << "Failed to apply " << NodeToString(proof.At(0).At(i)) << " to theorem " << NodeToString(theorem.At(i)) << std::endl;
                        return false;
                    }
                }
                return true;
            }
            if (proof.GetData() == "splitFunction") {
                if (theorem.GetType() != (size_t)TokenType::Function) {
                    std::cout << "Theorem " << NodeToString(theorem) << " should be a function" << std::endl;
                    return false;
                }
                const size_t size = theorem.At(0).GetSize();
                if (size != proof.At(0).GetSize()) {
                    std::cout << "Arity of theorem " << NodeToString(theorem) << " should match arity of splitFunction " << NodeToString(proof.At(0)) << std::endl;
                    return false;
                }
                for (size_t i = 0; i < size; i++) {
                    if (!CheckProof(theorem.At(0).At(i), proof.At(0).At(i), bindings)) {
                        std::cout << "Failed to apply " << NodeToString(proof.At(0).At(i)) << " to theorem " << NodeToString(theorem.At(0).At(i)) << std::endl;
                        return false;
                    }
                }
                return true;
            }
            if (proof.GetData() == "rewrite") {
                MathLib::ParserNode rule = InvokeFunction(proof.At(0), bindings);
                if (rule.GetType() != (size_t)TokenType::Function || rule.GetData() != "prove") {
                    std::cout << "Rule " << NodeToString(rule) << " should be proof" << std::endl;
                    return false;
                }
                const MathLib::ParserNode tmp = rule.At(0).At(0);
                rule = tmp;
                if (rule.GetType() == (size_t)TokenType::Equality) {
                    for (uint8_t i = 0; i < 2; i++) {
                        if (rule.At(i) == theorem) {
                            theorem = rule.At(!i);
                            return true;
                        }
                    }
                }
                else if (rule.GetType() == (size_t)TokenType::Implication) {
                    if (rule.At(1) == theorem) {
                        theorem = rule.AtUnsafe(0);
                        return true;
                    }
                }
                else {
                    std::cout << "Rule " << NodeToString(rule) << " should be an equality or implication" << std::endl;
                    return false;
                }
                std::cout << "Failed to apply rule " << NodeToString(rule) << " to theorem " << NodeToString(theorem) << std::endl;
                return false;
            }
            return CheckProof(theorem, InvokeFunction(proof, bindings), bindings);
        }
        default: {
            std::cout << "Invalid proof " << NodeToString(proof) << std::endl;
            return false;
        }
    }
}
MathLib::ParserNode ProveInternal(const MathLib::ParserNode& node, MathLib::Array<MathLib::ParserNode>& bindings) {
    switch ((TokenType)node.GetType()) {
        case TokenType::Comma: {
            MathLib::ParserNode newNode = node;
            for (uint8_t i = 0; i < 2; i++) {
                newNode.At(i) = ProveInternal(node.At(i), bindings);
                if (newNode.At(i).GetType() == SIZE_MAX) return MathLib::ParserNode();
            }
            return newNode;
        }
        case TokenType::Bind: {
            MathLib::ParserNode newNode = node;
            newNode.At(1) = ProveInternal(node.At(1), bindings);
            if (newNode.At(1).GetType() == SIZE_MAX || !bindings.Add(newNode)) return MathLib::ParserNode();
            if (newNode.At(1).GetType() == (size_t)TokenType::Function && newNode.At(1).GetData() == "prove")
                std::cout << "Proved: " << NodeToString(newNode.At(0)) << std::endl;
            return newNode;
        }
        case TokenType::Function: {
            const MathLib::ParserNode newNode = InvokeFunction(node, bindings);
            if (newNode.GetData() == "prove") {
                MathLib::ParserNode theorem = newNode.At(0).At(0);
                if (!CheckProof(theorem, newNode.At(0).At(1), bindings)) return MathLib::ParserNode();
            }
            return newNode;
        }
        default: return node;
    }
}
bool Prove(const MathLib::ParserNode& node) {
    MathLib::Array<MathLib::ParserNode> bindings;
    return ProveInternal(node, bindings).GetType() != SIZE_MAX;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        // TODO: leftAlternativity + flexibility => rightAlternativity (replace leftAlternativityAndCommutativityImpliesRightAlternativity)
        // TODO: rightAlternativity + flexibility => leftAlternativity (replace rightAlternativityAndCommutativityImpliesLeftAlternativity)
        // TODO: leftAlternativity + rightAlternativity => flexibility (replace associativityImpliesFlexibility)

        // TODO: leftBolIdentity a * (b * (a * c)) == (a * (b * a)) * c
        // TODO: rightBolIdentity ((c * a) * b) * a == c * ((a * b) * a)
        // TODO: associativity => leftBolIdentity
        // TODO: associativity => rightBolIdentity
        // TODO: leftBolIdentity => leftAlternativity
        // TODO: rightBolIdentity => rightAlternativity
        // TODO: leftBolIdentity + flexibility => rightBolIdentity
        // TODO: rightBolIdentity + flexibility => leftBolIdentity

        // TODO: leibnizIdentity (a * b) * c == (a * (b * c)) + ((a * c) * b)
        // TODO: jacobiIdentity (a * (b * c)) + (b * (c * a)) + (c * (a * b)) == 0
        if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <input file>");
        MathLib::Toolchain toolchain = MathLib::Toolchain(new MathLib::Lexer(MathLib::MakeArray<MathLib::LexerRule*>(
            new MathLib::WhitespaceLexerRule(SIZE_MAX),
            new MathLib::SingleCharLexerRule((size_t)TokenType::Comma, ','_M),
            new MathLib::StringLexerRule((size_t)TokenType::Equality, "=="_M),
            new MathLib::StringLexerRule((size_t)TokenType::Implication, "=>"_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::Bind, '='_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::ParenthesesStart, '('_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::ParenthesesEnd, ')'_M),
            new MathLib::IdentifierLexerRule((size_t)TokenType::Identifier, true)
        )), new MathLib::Parser(MathLib::MakeArray<MathLib::ParserLayer*>(
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Comma, (size_t)TokenType::Comma),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Bind, (size_t)TokenType::Bind),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Implication, (size_t)TokenType::Implication),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Equality, (size_t)TokenType::Equality),
            new MathLib::FunctionParserLayer((size_t)TokenType::Function, (size_t)TokenType::Identifier, (size_t)TokenType::ParenthesesStart),
            new MathLib::IdentityParserLayer((size_t)TokenType::Identifier, (size_t)TokenType::Identifier),
            new MathLib::UnwrapperParserLayer((size_t)TokenType::ParenthesesStart, (size_t)TokenType::ParenthesesEnd)
        )), new MathLib::IdentityEvaluator());
        MathLib::HostFileSystem fs;
        toolchain.LoadInput(fs.Open(MathLib::String(argv[1]), MathLib::OpenMode::Read).ReadUntil('\0'));
        std::cout << Prove(toolchain.GetNode()) << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}