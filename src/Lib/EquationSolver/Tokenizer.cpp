#include "Tokenizer.hpp"
#include "../Host.hpp"

namespace MathLib {
    /// @brief Converts a string to a node type
    /// @param str String to tokenize
    /// @param i Current position in the string
    /// @return Node type
    Node::Type GetType(const String& str, size_t& i) {
        StartBenchmark
        SkipWhiteSpace(str, i);
        if (str.GetSize() <= i) ReturnFromBenchmark(Node::Type::None);
        switch (str.At(i++)) {
            case ',': ReturnFromBenchmark(Node::Type::Comma);
            case '=': {
                if (i < str.GetSize() && str.At(i) == '=') {
                    i++;
                    ReturnFromBenchmark(Node::Type::LogicalEqual);
                }
                else ReturnFromBenchmark(Node::Type::Equal);
            }
            case '+': ReturnFromBenchmark(Node::Type::Add);
            case '-': ReturnFromBenchmark(Node::Type::Sub);
            case '*': ReturnFromBenchmark(Node::Type::Mul);
            case '/': ReturnFromBenchmark(Node::Type::Div);
            case '^': ReturnFromBenchmark(Node::Type::Pow);
            case '$': {
                if (i < str.GetSize() && str.At(i) == '=') {
                    i++;
                    ReturnFromBenchmark(Node::Type::DynamicEqual);
                }
                else ReturnFromBenchmark(Node::Type::Root);
            }
            case '!': {
                if (i < str.GetSize() && str.At(i) == '=') {
                    i++;
                    ReturnFromBenchmark(Node::Type::LogicalNotEqual);
                }
                else ReturnFromBenchmark(Node::Type::Factorial);
            }
            case '<': {
                if (i < str.GetSize() && str.At(i) == '=') {
                    i++;
                    ReturnFromBenchmark(Node::Type::LessThanEqual);
                }
                else ReturnFromBenchmark(Node::Type::LessThan);
            }
            case '>': {
                if (i < str.GetSize() && str.At(i) == '=') {
                    i++;
                    ReturnFromBenchmark(Node::Type::GreaterThanEqual);
                }
                else ReturnFromBenchmark(Node::Type::GreaterThan);
            }
            default: ReturnFromBenchmark(Node::Type::None);
        }
    }
    Node* TokenizeInternal(const String& str, size_t& i);
    /// @brief Tokenizes string into nodes
    /// @param str String to tokenize
    /// @param i Current position in the string
    /// @return Tokenized string
    Node* TokenizeComma(const String& str, size_t& i) {
        StartBenchmark
        Node* ret = TokenizeInternal(str, i);
        SkipWhiteSpace(str, i);
        while (i < str.GetSize() && str.At(i) == ',')
            ret = new Node(GetType(str, i), "", ret, TokenizeInternal(str, i));
        ReturnFromBenchmark(ret);
    }
    /// @brief Tokenizes string into nodes
    /// @param str String to tokenize
    /// @param i Current position in the string
    /// @return Tokenized string
    Node* TokenizeData(const String& str, size_t& i) {
        StartBenchmark
        SkipWhiteSpace(str, i);
        if (IsDigit(str.At(i))) {
            String ret = "";
            while (i < str.GetSize() && (IsDigit(str.At(i)) || str.At(i) == '.' || str.At(i) == 'e')) ret += str.At(i++);
            if (str.At(i) == 'i') {
                i++;
                ReturnFromBenchmark(new Node(Node::Type::Constant, String("0 + ") + ret + 'i'));
            }
            ReturnFromBenchmark(new Node(Node::Type::Constant, ret));
        }
        else if (IsAlpha(str.At(i))) {
            String name = "";
            while (i < str.GetSize() && IsAlphaDigit(str.At(i))) name += str.At(i++);
            SkipWhiteSpace(str, i);
            if (i < str.GetSize() && str.At(i) == ':') {
                i++;
                ReturnFromBenchmark(new Node(Node::Type::Variable, name, TokenizeData(str, i)));
            }
            else if (i < str.GetSize() && str.At(i) == '(') {
                i++;
                Node* ret = TokenizeComma(str, i);
                if (!ret) ReturnFromBenchmark(nullptr);
                SkipWhiteSpace(str, i);
                if (str.At(i) != ')') {
                    delete ret;
                    ReturnFromBenchmark(nullptr);
                }
                i++;
                for (size_t j = 0; j < SizeOfArray(Node::keywords); j++)
                    if (name == Node::keywords[j]) ReturnFromBenchmark(new Node(Node::Type::Keyword, name, ret));
                SkipWhiteSpace(str, i);
                if (i < str.GetSize() && str.At(i) == ':') {
                    i++;
                    ReturnFromBenchmark(new Node(Node::Type::Function, name, ret, TokenizeData(str, i)));
                }
                else ReturnFromBenchmark(new Node(Node::Type::Function, name, ret));
            }
            else if (name == "i") ReturnFromBenchmark(new Node(Node::Type::Constant, "0 + 1i"))
            else ReturnFromBenchmark(new Node(Node::Type::Variable, name));
        }
        else if (str.At(i) == '"') {
            i++;
            String ret = "";
            while (str.At(i) != '"') ret += str.At(i++);
            i++;
            ReturnFromBenchmark(new Node(Node::Type::String, ret));
        }
        else if (str.At(i) == '{') {
            i++;
            Node* ret = TokenizeComma(str, i);
            if (!ret) ReturnFromBenchmark(nullptr);
            SkipWhiteSpace(str, i);
            if (str.At(i) != '}') {
                delete ret;
                ReturnFromBenchmark(nullptr);
            }
            i++;
            ReturnFromBenchmark(new Node(Node::Type::Array, "", ret));
        }
        else if (str.At(i) == '[') {
            i++;
            SkipWhiteSpace(str, i);
            bool runtime = false;
            if (str.At(i) == '!') {
                runtime = true;
                i++;
            }
            Node* ret = TokenizeComma(str, i);
            if (!ret) ReturnFromBenchmark(nullptr);
            SkipWhiteSpace(str, i);
            if (str.At(i) != ']') {
                delete ret;
                ReturnFromBenchmark(nullptr);
            }
            i++;
            ReturnFromBenchmark(new Node(Node::Type::Program, ToString(runtime), ret));
        }
        else if (str.At(i) == '(') {
            i++;
            Node* ret = TokenizeInternal(str, i);
            if (!ret) ReturnFromBenchmark(nullptr);
            SkipWhiteSpace(str, i);
            if (str.At(i) != ')') {
                delete ret;
                ReturnFromBenchmark(nullptr);
            }
            i++;
            ReturnFromBenchmark(ret);
        }
        else if (str.At(i) == '|') {
            i++;
            Node* ret = TokenizeInternal(str, i);
            if (!ret) ReturnFromBenchmark(nullptr);
            if (str.At(i) != '|') {
                delete ret;
                ReturnFromBenchmark(nullptr);
            }
            i++;
            ReturnFromBenchmark(new Node(Node::Type::Absolute, "", ret));
        }
        else if (str.At(i) == '-') {
            i++;
            Node* ret = TokenizeInternal(str, i);
            ReturnFromBenchmark(ret ? new Node(Node::Type::Mul, "", new Node(Node::Type::Constant, "-1"), ret) : nullptr);
        }
        else ReturnFromBenchmark(nullptr);
    }

    #define TokenizeLayer(func, req, next)                                      \
    Node* Tokenize##func(const String& str, size_t& i) {                        \
        StartBenchmark                                                          \
        Node* ret = Tokenize##next(str, i);                                     \
        SkipWhiteSpace(str, i);                                                 \
        size_t tmp = i;                                                         \
        Node::Type type = GetType(str, i);                                      \
        i = tmp;                                                                \
        while (i < str.GetSize() && (req)) {                                    \
            ret = new Node(GetType(str, i), "", ret, Tokenize##next(str, i));   \
            if (!ret) ReturnFromBenchmark(nullptr);                             \
            tmp = i;                                                            \
            type = GetType(str, i);                                             \
            i = tmp;                                                            \
        }                                                                       \
        ReturnFromBenchmark(ret);                                               \
    }
    /// @brief Tokenizes string into nodes
    /// @param str String to tokenize
    /// @param i Current position in the string
    /// @return Tokenized string
    Node* TokenizeIndex(const String& str, size_t& i) {
        StartBenchmark
        Node* ret = TokenizeData(str, i);
        SkipWhiteSpace(str, i);
        if (i < str.GetSize() && str.At(i) == '[') {
            i++;
            ret = new Node(Node::Type::Index, "", ret, TokenizeInternal(str, i));
            SkipWhiteSpace(str, i);
            if (str.At(i) != ']') {
                delete ret;
                ReturnFromBenchmark(nullptr);
            }
            i++;
        }
        ReturnFromBenchmark(ret);
    }
    /// @brief Tokenizes string into nodes
    /// @param str String to tokenize
    /// @param i Current position in the string
    /// @return Tokenized string
    Node* TokenizeFactorial(const String& str, size_t& i) {
        StartBenchmark
        Node* ret = TokenizeIndex(str, i);
        size_t tmp = i;
        const Node::Type type = GetType(str, i);
        if (type == Node::Type::Factorial) ret = new Node(type, "", ret);
        else i = tmp;
        ReturnFromBenchmark(ret);
    }
    TokenizeLayer(Exponentiation, type == Node::Type::Pow || type == Node::Type::Root, Factorial)
    TokenizeLayer(Multiplication, type == Node::Type::Mul || type == Node::Type::Div, Exponentiation)
    TokenizeLayer(Addition, type == Node::Type::Add || type == Node::Type::Sub, Multiplication)
    TokenizeLayer(Relationality, type == Node::Type::LessThan || type == Node::Type::GreaterThan || type == Node::Type::LessThanEqual || type == Node::Type::GreaterThanEqual, Addition)
    TokenizeLayer(Equality, type == Node::Type::LogicalEqual || type == Node::Type::LogicalNotEqual, Relationality)
    TokenizeLayer(Assignment, type == Node::Type::DynamicEqual || type == Node::Type::Equal, Equality)
    /// @brief Tokenizes string into nodes
    /// @param str String to tokenize
    /// @param i Current position in the string
    /// @return Tokenized string
    Node* TokenizeInternal(const String& str, size_t& i) {
        StartBenchmark
        ReturnFromBenchmark(TokenizeAssignment(str, i));
    }
    Node* Tokenize(const String& str) {
        StartBenchmark
        size_t i = 0;
        Node* ret = TokenizeInternal(str, i);
        while (i < str.GetSize() && ret)
            ret = new Node(Node::Type::Comma, "", ret, TokenizeInternal(str, i));
        ReturnFromBenchmark(ret);
    }
}