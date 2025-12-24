#include "LambdaTerm.hpp"
#include <Compiler/Parser/RightBinaryParserLayer.hpp>
#include <Compiler/Parser/LeftBinaryParserLayer.hpp>
#include <Compiler/Parser/UnwrapperParserLayer.hpp>
#include <Compiler/Parser/IdentityParserLayer.hpp>
#include <Compiler/Lexer/IdentifierLexerRule.hpp>
#include <Compiler/Lexer/SingleCharLexerRule.hpp>
#include <Compiler/Lexer/WhitespaceLexerRule.hpp>
#include <Compiler/Parser/KeywordParserLayer.hpp>
#include <Compiler/Lexer/StringLexerRule.hpp>
#include <Compiler/IdentityEvaluator.hpp>
#include <Libc/HostFileSystem.hpp>
#include <Compiler/Toolchain.hpp>
#include <FileSystem/Path.hpp>
#include <String.hpp>
#include <iostream>

enum class TokenType : uint8_t {
    ParenthesesStart,
    ParenthesesEnd,
    Variable,
    Abstraction,
    Application,
    Definition,
    Comma,
    String,
    Pattern,
};
MathLib::Array<MathLib::ParserNode> CommaToArray(const MathLib::ParserNode& node) {
    if (node.GetType() != (size_t)TokenType::Comma) return MathLib::MakeArray<MathLib::ParserNode>(node);
    MathLib::Array<MathLib::ParserNode> tmp = CommaToArray(node.At(0));
    tmp += CommaToArray(node.At(1));
    return tmp;
}
LambdaTerm FromNodeInternal(const MathLib::ParserNode& node) {
    switch ((TokenType)node.GetType()) {
        case TokenType::Variable: return LambdaTerm(node.GetData(), false);
        case TokenType::String: {
            const MathLib::String tmp = node.GetData();
            return LambdaTerm(MathLib::SubString(tmp, 1, tmp.GetSize() - 2), true);
        }
        case TokenType::Abstraction: return LambdaTerm(FromNodeInternal(node.At(1)), node.At(0).GetData());
        case TokenType::Application: return LambdaTerm(FromNodeInternal(node.At(0)), FromNodeInternal(node.At(1)));
        case TokenType::Definition: return LambdaTerm(node.At(0).GetData(), FromNodeInternal(node.At(1)));
        case TokenType::Pattern: {
            const MathLib::Array<MathLib::ParserNode> cases = CommaToArray(node.At(0));
            const size_t size = cases.GetSize();
            MathLib::Array<LambdaTerm> terms = MathLib::Array<LambdaTerm>(size * 2);
            for (size_t i = 0; i < size; i++) {
                if (cases.At(i).GetType() != (size_t)TokenType::Abstraction) return LambdaTerm();
                terms.At(i * 2) = FromNodeInternal(cases.At(i).At(0));
                terms.At(i * 2 + 1) = FromNodeInternal(cases.At(i).At(1));
            }
            return LambdaTerm(terms);
        }
        default: return LambdaTerm();
    }
}
MathLib::Array<LambdaTerm> FromNode(const MathLib::ParserNode& node) {
    switch ((TokenType)node.GetType()) {
        case TokenType::Comma: {
            MathLib::Array<LambdaTerm> tmp = FromNode(node.At(0));
            tmp += FromNode(node.At(1));
            return tmp;
        }
        default: return MathLib::MakeArray<LambdaTerm>(FromNodeInternal(node));
    }
}
struct GroupedLexerRule : MathLib::LexerRule {
    GroupedLexerRule(size_t type, char start, char end) : MathLib::LexerRule(type), start(start), end(end) {}
    virtual void Match(const MathLib::Sequence<char>& str, size_t& i) const override {
        if (str.At(i) != start) return;
        const size_t tmp = str.Find(end, i + 1);
        if (tmp != SIZE_MAX) i = tmp + 1;
    }

    private:
    char start;
    char end;
};
struct PatternParserLayer : MathLib::ParserLayer {
    [[nodiscard]] virtual MathLib::ParserNode Parse(const MathLib::Function<MathLib::ParserNode>& root, const MathLib::Function<MathLib::ParserNode>& next, const MathLib::Sequence<MathLib::Token>& tokens, size_t& i) const override {
        if (!tokens.At(i).CheckType((size_t)TokenType::Variable) || tokens.At(i).GetValue() != "pattern") return next();
        i++;
        if (!tokens.At(i).CheckType((size_t)TokenType::ParenthesesStart)) return MathLib::ParserNode();
        i++;
        const MathLib::ParserNode ret = MathLib::ParserNode((size_t)TokenType::Pattern, ""_M, MathLib::MakeArray<MathLib::ParserNode>(root()));
        if (!tokens.At(i).CheckType((size_t)TokenType::ParenthesesEnd)) return MathLib::ParserNode();
        i++;
        return ret;
    }
};
MathLib::String Preprocess(MathLib::FileSystem& fileSystem, const MathLib::Sequence<char>& path) {
    const MathLib::String basePath = MathLib::RemoveLastPathElement(path);
    MathLib::String str = fileSystem.Open(path, MathLib::OpenMode::Read).ReadUntil('\0');
    const size_t size = str.GetSize();
    MathLib::String ret;
    for (size_t i = 0; i < size;) {
        const char tmp = str.At(i++);
        if (tmp == '$') {
            const size_t index = str.Find('$', i);
            if (index == SIZE_MAX) return "";
            ret += Preprocess(fileSystem, MathLib::AppendPath(basePath, MathLib::SubString(str, i, index - i)));
            i = index + 1;
        }
        else ret += tmp;
    }
    return ret;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <input file>");
        MathLib::Toolchain toolchain = MathLib::Toolchain(new MathLib::Lexer(MathLib::MakeArray<MathLib::LexerRule*>(
            new MathLib::WhitespaceLexerRule(SIZE_MAX),
            new GroupedLexerRule((size_t)TokenType::String, '"', '"'),
            new MathLib::SingleCharLexerRule((size_t)TokenType::ParenthesesStart, '('_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::ParenthesesEnd, ')'_M),
            new MathLib::IdentifierLexerRule((size_t)TokenType::Variable, true),
            new MathLib::StringLexerRule((size_t)TokenType::Abstraction, "->"_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::Application, '.'_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::Comma, ','_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::Definition, '='_M)
        )), new MathLib::Parser(MathLib::MakeArray<MathLib::ParserLayer*>(
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Comma, (size_t)TokenType::Comma),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Definition, (size_t)TokenType::Definition),
            new MathLib::RightBinaryParserLayer((size_t)TokenType::Abstraction, (size_t)TokenType::Abstraction),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Application, (size_t)TokenType::Application),
            new MathLib::UnwrapperParserLayer((size_t)TokenType::ParenthesesStart, (size_t)TokenType::ParenthesesEnd),
            new MathLib::IdentityParserLayer((size_t)TokenType::String, (size_t)TokenType::String),
            new PatternParserLayer(),
            new MathLib::IdentityParserLayer((size_t)TokenType::Variable, (size_t)TokenType::Variable)
        )), new MathLib::IdentityEvaluator());
        MathLib::HostFileSystem fs;
        toolchain.LoadInput(Preprocess(fs, MathLib::String(argv[1])));
        const MathLib::Array<LambdaTerm> bindings = FromNode(toolchain.GetNode());
        const size_t size = bindings.GetSize();
        size_t main = SIZE_MAX;
        for (size_t i = 0; i < size; i++) {
            if (i) std::cout << ",\n";
            std::cout << bindings.At(i);
            if (bindings.At(i).value == "Main") main = i;
        }
        std::cout << std::endl;
        if (main == SIZE_MAX) MathLib::Panic("No Main function specified");
        LambdaTerm ret = bindings.At(main).Run(bindings);
        std::cout << "Output: " << ret << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}