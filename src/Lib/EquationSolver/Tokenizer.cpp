#include "Tokenizer.hpp"
#include "../Host.hpp"

void SkipWhiteSpace(const String& str, size_t& i) {
    while (i < str.GetSize() && IsWhiteSpace(str[i])) i++;
}
/// @brief Converts a string to a node type
/// @param str String to tokenize
/// @param i Current position in the string
/// @return Node type
Node::Type GetType(const String& str, size_t& i) {
    SkipWhiteSpace(str, i);
    switch (str[i++]) {
        case ',': return Node::Type::Comma;
        case '=': return Node::Type::Equal;
        case '+': return Node::Type::Add;
        case '-': return Node::Type::Sub;
        case '*': return Node::Type::Mul;
        case '/': return Node::Type::Div;
        case '^': return Node::Type::Pow;
        case '$': return Node::Type::Root;
        case '!': return Node::Type::Factorial;
        case ':': {
            if (str[i++] == '=') return Node::Type::DynamicEqual;
            else return Node::Type::None;
        }
        default: return Node::Type::None;
    }
}
Node* TokenizeInternal(const String& str, size_t& i);
/// @brief Tokenizes string into nodes
/// @param str String to tokenize
/// @param i Current position in the string
/// @return Tokenized string
Node* TokenizeComma(const String& str, size_t& i) {
    Node* ret = TokenizeInternal(str, i);
    SkipWhiteSpace(str, i);
    while (i < str.GetSize() && str[i] == ',')
        ret = new Node(GetType(str, i), "", ret, TokenizeInternal(str, i));
    return ret;
}
/// @brief Tokenizes string into nodes
/// @param str String to tokenize
/// @param i Current position in the string
/// @return Tokenized string
Node* TokenizeData(const String& str, size_t& i) {
    SkipWhiteSpace(str, i);
    if (IsDigit(str[i])) {
        String ret = "";
        while (i < str.GetSize() && (IsDigit(str[i]) || str[i] == '.' || str[i] == 'e')) ret += str[i++];
        if (str[i] == 'i') {
            i++;
            return new Node(Node::Type::Constant, String("0 + ") + ret + 'i');
        }
        return new Node(Node::Type::Constant, ret);
    }
    else if (IsAlpha(str[i])) {
        String name = "";
        while (i < str.GetSize() && IsAlphaDigit(str[i])) name += str[i++];
        SkipWhiteSpace(str, i);
        if (i < str.GetSize() && str[i] == ':') {
            i++;
            return new Node(Node::Type::Variable, name, TokenizeData(str, i));
        }
        else if (i < str.GetSize() && str[i] == '(') {
            i++;
            Node* ret = TokenizeComma(str, i);
            if (ret == nullptr) return nullptr;
            SkipWhiteSpace(str, i);
            if (str[i] != ')') {
                delete ret;
                return nullptr;
            }
            i++;
            if (name == "integral") return new Node(Node::Type::Integral, "", ret);
            else if (name == "summation") return new Node(Node::Type::Summation, "", ret);
            else if (name == "product") return new Node(Node::Type::Product, "", ret);
            else {
                SkipWhiteSpace(str, i);
                if (i < str.GetSize() && str[i] == ':') {
                    i++;
                    return new Node(Node::Type::Function, name, ret, TokenizeData(str, i));
                }
                else return new Node(Node::Type::Function, name, ret);
            }
        }
        else if (name == "i") return new Node(Node::Type::Constant, "0 + 1i");
        else return new Node(Node::Type::Variable, name);
    }
    else if (str[i] == '"') {
        i++;
        String ret = "";
        while (str[i] != '"') ret += str[i++];
        i++;
        return new Node(Node::Type::String, ret);
    }
    else if (str[i] == '{') {
        i++;
        Node* ret = TokenizeComma(str, i);
        if (ret == nullptr) return nullptr;
        SkipWhiteSpace(str, i);
        if (str[i] != '}') {
            delete ret;
            return nullptr;
        }
        i++;
        return new Node(Node::Type::Array, "", ret);
    }
    else if (str[i] == '[') {
        i++;
        SkipWhiteSpace(str, i);
        bool runtime = false;
        if (str[i] == '!') {
            runtime = true;
            i++;
        }
        Node* ret = TokenizeComma(str, i);
        if (ret == nullptr) return nullptr;
        SkipWhiteSpace(str, i);
        if (str[i] != ']') {
            delete ret;
            return nullptr;
        }
        i++;
        return new Node(Node::Type::Program, ToString(runtime), ret);
    }
    else if (str[i] == '(') {
        i++;
        Node* ret = TokenizeInternal(str, i);
        if (ret == nullptr) return nullptr;
        SkipWhiteSpace(str, i);
        if (str[i] != ')') {
            delete ret;
            return nullptr;
        }
        i++;
        return ret;
    }
    else if (str[i] == '|') {
        i++;
        Node* ret = TokenizeInternal(str, i);
        if (ret == nullptr) return nullptr;
        if (str[i] != '|') {
            delete ret;
            return nullptr;
        }
        i++;
        return new Node(Node::Type::Absolute, "", ret);
    }
    else if (str[i] == '-') {
        i++;
        Node* ret = TokenizeInternal(str, i);
        if (ret == nullptr) return nullptr;
        return new Node(Node::Type::Mul, "", new Node(Node::Type::Constant, "-1"), ret);
    }
    else return nullptr;
}

#define TokenizeLayer(func, req, next)                                      \
Node* Tokenize##func(const String& str, size_t& i) {                        \
    Node* ret = Tokenize##next(str, i);                                     \
    SkipWhiteSpace(str, i);                                                 \
    while (i < str.GetSize() && (req)) {                                    \
        ret = new Node(GetType(str, i), "", ret, Tokenize##next(str, i));   \
        if (ret == nullptr) return nullptr;                                 \
    }                                                                       \
    return ret;                                                             \
}
/// @brief Tokenizes string into nodes
/// @param str String to tokenize
/// @param i Current position in the string
/// @return Tokenized string
Node* TokenizeIndex(const String& str, size_t& i) {
    Node* ret = TokenizeData(str, i);
    SkipWhiteSpace(str, i);
    if (i < str.GetSize() && str[i] == '[') {
        i++;
        ret = new Node(Node::Type::Index, "", ret, TokenizeInternal(str, i));
        SkipWhiteSpace(str, i);
        if (str[i] != ']') {
            delete ret;
            return nullptr;
        }
        i++;
    }
    return ret;
}
/// @brief Tokenizes string into nodes
/// @param str String to tokenize
/// @param i Current position in the string
/// @return Tokenized string
Node* TokenizeFactorial(const String& str, size_t& i) {
    Node* ret = TokenizeIndex(str, i);
    if (i < str.GetSize() && str[i] == '!') ret = new Node(GetType(str, i), "", ret);
    return ret;
}
TokenizeLayer(Exponentiation, str[i] == '^' || str[i] == '$', Factorial)
TokenizeLayer(Multiplication, str[i] == '*' || str[i] == '/', Exponentiation)
TokenizeLayer(Addition, str[i] == '+' || str[i] == '-', Multiplication)
TokenizeLayer(Equality, (str[i] == ':' && str[i + 1] == '=') || str[i] == '=', Addition)
/// @brief Tokenizes string into nodes
/// @param str String to tokenize
/// @param i Current position in the string
/// @return Tokenized string
Node* TokenizeInternal(const String& str, size_t& i) {
    return TokenizeEquality(str, i);
}
Node* Tokenize(const String& str) {
    size_t i = 0;
    Node* ret = TokenizeInternal(str, i);
    while (i < str.GetSize() && ret != nullptr) ret = new Node(Node::Type::Comma, "", ret, TokenizeInternal(str, i));
    return ret;
}