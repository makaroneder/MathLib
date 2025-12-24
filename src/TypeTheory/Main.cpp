#include <Compiler/Parser/RightBinaryParserLayer.hpp>
#include <Compiler/Parser/LeftBinaryParserLayer.hpp>
#include <Compiler/Parser/UnwrapperParserLayer.hpp>
#include <Compiler/Parser/IdentityParserLayer.hpp>
#include <Compiler/Lexer/IdentifierLexerRule.hpp>
#include <Compiler/Lexer/SingleCharLexerRule.hpp>
#include <Compiler/Lexer/WhitespaceLexerRule.hpp>
#include <Compiler/Lexer/StringLexerRule.hpp>
#include <Compiler/IdentityEvaluator.hpp>
#include <Interfaces/Comparable.hpp>
#include <Interfaces/Printable.hpp>
#include <Libc/HostFileSystem.hpp>
#include <Compiler/Toolchain.hpp>
#include <iostream>

[[nodiscard]] MathLib::String WrapInParentheses(const MathLib::String& value, bool wrap) {
    return wrap ? '('_M + value + ')' : value;
}
struct Term;
struct Term : MathLib::Comparable<Term>, MathLib::Printable {
    enum class Type : uint8_t {
        None,
        Variable,
        Comma,
        Assignment,
        TypeDeclaration,
        TypeAbstraction,
        TypeApplication,
        Abstraction,
        Application,
    };
    MathLib::Array<Term> children;
    MathLib::String name;
    Type type;

    Term(void) : children(), name(), type(Type::None) {}
    Term(const MathLib::String& name) : children(), name(name), type(Type::Variable) {}
    Term(Type type, const MathLib::Array<Term>& children) : children(children), name(), type(type) {}
    Term(Type type, const Term& a, const Term& b) : children(MathLib::MakeArray<Term>(a, b)), name(), type(type) {}
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override {
        return MathLib::CollectionToString(padding) + ToStringInternal(true);
    }
    [[nodiscard]] Term Replace(const Term& old, const Term& replacement) const {
        if (Equals(old)) return replacement;
        Term ret = *this;
        for (Term& child : ret.children) child = child.Replace(old, replacement);
        return ret;
    }

    protected:
    [[nodiscard]] virtual bool Equals(const Term& other) const override {
        return type == other.type && name == other.name && children == other.children;
    }

    private:
    [[nodiscard]] MathLib::String ToStringInternal(bool root) const {
        switch (type) {
            case Type::Variable: return name;
            case Type::Comma: return WrapInParentheses(children.At(0).ToStringInternal(children.At(0).type >= type) + ", " + children.At(1).ToStringInternal(children.At(1).type > type), !root);
            case Type::Assignment: return WrapInParentheses(children.At(0).ToStringInternal(children.At(0).type >= type) + " = " + children.At(1).ToStringInternal(children.At(1).type > type), !root);
            case Type::TypeDeclaration: return WrapInParentheses(children.At(0).ToStringInternal(children.At(0).type >= type) + " : " + children.At(1).ToStringInternal(children.At(1).type > type), !root);
            case Type::TypeAbstraction: return WrapInParentheses(children.At(0).ToStringInternal(children.At(0).type > type) + " => " + children.At(1).ToStringInternal(children.At(1).type >= type), !root);
            case Type::TypeApplication: return WrapInParentheses(children.At(0).ToStringInternal(children.At(0).type >= type) + " ^ " + children.At(1).ToStringInternal(children.At(1).type > type), !root);
            case Type::Abstraction: return WrapInParentheses(children.At(0).ToStringInternal(children.At(0).type > type) + " -> " + children.At(1).ToStringInternal(children.At(1).type >= type), !root);
            case Type::Application: return WrapInParentheses(children.At(0).ToStringInternal(children.At(0).type >= type) + " . " + children.At(1).ToStringInternal(children.At(1).type > type), !root);
            default: return "";
        }
    }
};
struct Context : MathLib::Printable {
    Context(void) : theorems(), parent(nullptr) {}
    Context(const MathLib::Array<Term>& theorems) : theorems(theorems), parent(nullptr) {}

    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override {
        const MathLib::String padd = MathLib::CollectionToString(padding);
        const MathLib::String padd2 = padd + '\t';
        return "{\n"_M + ToStringInternal(padd2) + padd + '}';
    }
    [[nodiscard]] Term GetTrivialTypeOf(const Term& term) const {
        if (parent) {
            const Term tmp = parent->GetTrivialTypeOf(term);
            if (tmp.type != Term::Type::None) return tmp;
        }
        for (const Term& theorem : theorems) {
            switch (theorem.type) {
                case Term::Type::TypeDeclaration: {
                    // TODO: A => b : A => B <=> b : B
                    // TODO: a ^ b : B <=> a : A => B && b : Typeof(A)
                    // TODO: a -> b : A -> B <=> a : A && b : B
                    // TODO: a . b : B <=> a : A -> B && b : A
                    if (theorem.children.At(0) == term) return theorem.children.At(1);
                    break;
                }
                case Term::Type::Assignment: {
                    const Term typeDeclaration = theorem.children.At(0);
                    if (theorem.children.At(1) == term || typeDeclaration.children.At(0) == term) return typeDeclaration.children.At(1);
                    break;
                }
                default: break;
            }
        }
        return Term();
    }
    [[nodiscard]] Term GetTypeOf(const Term& term) const {
        const Term tmp = GetTrivialTypeOf(term);
        if (tmp.type != Term::Type::None) return tmp;
        switch (term.type) {
            case Term::Type::TypeAbstraction: return Term(Term::Type::TypeAbstraction, term.children.At(0), GetTypeOf(term.children.At(1)));
            case Term::Type::TypeApplication: {
                const Term a = GetTypeOf(term.children.At(0));
                if (a.type != Term::Type::TypeAbstraction) return Term();
                return a.children.At(1).Replace(a.children.At(0), term.children.At(1));
            }
            case Term::Type::Abstraction: {
                if (term.children.At(0).type != Term::Type::TypeDeclaration) return Term();
                return Term(Term::Type::Abstraction, term.children.At(0).children.At(1), Context(MathLib::MakeArray<Term>(
                    term.children.At(0)
                ), this).GetTypeOf(term.children.At(1)));
            }
            case Term::Type::Application: {
                const Term a = GetTypeOf(term.children.At(0));
                if (a.type != Term::Type::Abstraction) return Term();
                const Term b = GetTypeOf(term.children.At(1));
                if (a.children.At(0) != b) return Term();
                return a.children.At(1);
            }
            default: return Term();
        }
    }
    [[nodiscard]] bool CanBeTypeOf(const Term& term, const Term& type) const {
        return GetTypeOf(term) == type;
    }
    [[nodiscard]] bool TypeCheck(const Term& term) {
        switch (term.type) {
            case Term::Type::Comma: return TypeCheck(term.children.At(0)) && TypeCheck(term.children.At(1));
            case Term::Type::Assignment: return CanBeTypeOf(term.children.At(1), term.children.At(0).children.At(1)) && theorems.Add(term);
            case Term::Type::TypeDeclaration: return theorems.Add(term);
            default: return false;
        }
    }

    private:
    Context(const MathLib::Array<Term>& theorems, const Context* const parent) : theorems(theorems), parent(parent) {}
    [[nodiscard]] MathLib::String ToStringInternal(const MathLib::String& padding) const {
        MathLib::String ret;
        for (const Term& theorem : theorems) ret += theorem.ToString(padding) + '\n';
        if (parent) ret += parent->ToStringInternal(padding);
        return ret;
    }

    MathLib::Array<Term> theorems;
    const Context* const parent;
};
enum class TokenType : uint8_t {
    ParenthesesStart,
    ParenthesesEnd,
    Variable,
    Comma,
    Assignment,
    TypeDeclaration,
    Abstraction,
    Application,
};
[[nodiscard]] Term NodeToTerm(const MathLib::ParserNode& node) {
    switch ((TokenType)node.GetType()) {
        case TokenType::Variable: return Term(node.GetData());
        case TokenType::Comma:
        case TokenType::Assignment:
        case TokenType::TypeDeclaration:
        case TokenType::Abstraction:
        case TokenType::Application: {
            Term::Type type = Term::Type::None;
            switch ((TokenType)node.GetType()) {
                case TokenType::Comma: {
                    type = Term::Type::Comma;
                    break;
                }
                case TokenType::Assignment: {
                    type = Term::Type::Assignment;
                    break;
                }
                case TokenType::TypeDeclaration: {
                    type = Term::Type::TypeDeclaration;
                    break;
                }
                case TokenType::Abstraction: {
                    type = node.GetData() == "->" ? Term::Type::Abstraction : Term::Type::TypeAbstraction;
                    break;
                }
                case TokenType::Application: {
                    type = node.GetData() == "." ? Term::Type::Application : Term::Type::TypeApplication;
                    break;
                }
                default: break;
            }
            return Term(type, NodeToTerm(node.At(0)), NodeToTerm(node.At(1)));
        }
        default: return Term();
    }
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
            new MathLib::SingleCharLexerRule((size_t)TokenType::ParenthesesStart, '('_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::ParenthesesEnd, ')'_M),
            new MathLib::IdentifierLexerRule((size_t)TokenType::Variable, true),
            new MathLib::StringLexerRule((size_t)TokenType::Abstraction, MathLib::MakeArray<MathLib::String>("->", "=>")),
            new MathLib::StringLexerRule((size_t)TokenType::Application, MathLib::MakeArray<MathLib::String>('.', '^')),
            new MathLib::SingleCharLexerRule((size_t)TokenType::Comma, ','_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::TypeDeclaration, ':'_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::Assignment, '='_M)
        )), new MathLib::Parser(MathLib::MakeArray<MathLib::ParserLayer*>(
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Comma, (size_t)TokenType::Comma),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Assignment, (size_t)TokenType::Assignment),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::TypeDeclaration, (size_t)TokenType::TypeDeclaration),
            new MathLib::RightBinaryParserLayer((size_t)TokenType::Abstraction, (size_t)TokenType::Abstraction),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Application, (size_t)TokenType::Application),
            new MathLib::UnwrapperParserLayer((size_t)TokenType::ParenthesesStart, (size_t)TokenType::ParenthesesEnd),
            new MathLib::IdentityParserLayer((size_t)TokenType::Variable, (size_t)TokenType::Variable)
        )), new MathLib::IdentityEvaluator());
        MathLib::HostFileSystem fs;
        toolchain.LoadInput(fs.Open(MathLib::String(argv[1]), MathLib::OpenMode::Read).ReadUntil('\0'));
        Context context;
        if (!context.TypeCheck(NodeToTerm(toolchain.GetNode()))) MathLib::Panic("Failed to type check the program");
        std::cout << context << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}