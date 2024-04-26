#include "Tokenizer.hpp"
#include <string.h>
#include <iostream>

bool IsWhiteSpace(char chr) {
    return chr == ' ' || chr == '\t' || chr == '\n' || chr == '\r';
}
void SkipWhiteSpace(const std::string str, size_t& i) {
    while (IsWhiteSpace(str[i])) i++;
}
/// @brief Converts a string to a node type
/// @param str String to tokenize
/// @param i Current position in the string
/// @return Node type
Node::Type GetType(const std::string str, size_t& i) {
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
        default: return Node::Type::None;
    }
}
Node* TokenizeInternal(const std::string str, size_t& i);
/// @brief Tokenizes string into nodes
/// @param str String to tokenize
/// @param i Current position in the string
/// @return Tokenized string
Node* TokenizeComma(const std::string str, size_t& i) {
    Node* ret = TokenizeInternal(str, i);
    SkipWhiteSpace(str, i);
    while (i < str.size() && str[i] == ',')
        ret = new Node(GetType(str, i), "", ret, TokenizeInternal(str, i));
    return ret;
}
/// @brief Tokenizes string into nodes
/// @param str String to tokenize
/// @param i Current position in the string
/// @return Tokenized string
Node* TokenizeData(const std::string str, size_t& i) {
    SkipWhiteSpace(str, i);
    if (isdigit(str[i])) {
        std::string ret = "";
        if ((i + 1) < str.size() && str[i] == '0' && str[i + 1] == 'x') {
            i += 2;
            while (i < str.size() && isxdigit(str[i])) ret += str[i++];
            ret = std::to_string(std::stoull(ret, nullptr, 16));
        }
        else while (i < str.size() && (isdigit(str[i]) || str[i] == '.' || str[i] == 'e')) ret += str[i++];
        if (str[i] == 'i') {
            i++;
            return new Node(Node::Type::ComplexConstant, "(0, " + ret + ')');
        }
        return new Node(Node::Type::Constant, ret);
    }
    else if (isalpha(str[i])) {
        std::string name = "";
        while (isalnum(str[i])) name += str[i++];
        SkipWhiteSpace(str, i);
        if (str[i] == '(') {
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
                if (str[i] == ':') {
                    i++;
                    Node* inputSet = TokenizeData(str, i);
                    if (inputSet == nullptr) {
                        delete ret;
                        return nullptr;
                    }
                    SkipWhiteSpace(str, i);
                    if (str[i] != '-' || str[i + 1] != '>') {
                        delete inputSet;
                        delete ret;
                        return nullptr;
                    }
                    i += 2;
                    Node* outputSet = TokenizeData(str, i);
                    if (outputSet == nullptr) {
                        delete inputSet;
                        delete ret;
                        return nullptr;
                    }
                    return new Node(Node::Type::Function, name, ret, new Node(Node::Type::Comma, "", inputSet, outputSet));
                }
                else return new Node(Node::Type::Function, name, ret);
            }
        }
        else if (name == "i") return new Node(Node::Type::ComplexConstant, "(0, 1)");
        else return new Node(Node::Type::Variable, name);
    }
    else if (str[i] == '"') {
        i++;
        std::string ret = "";
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
Node* Tokenize##func(const std::string str, size_t& i) {                    \
    Node* ret = Tokenize##next(str, i);                                     \
    SkipWhiteSpace(str, i);                                                 \
    while (i < str.size() && (req)) {                                       \
        ret = new Node(GetType(str, i), "", ret, Tokenize##next(str, i));   \
        if (ret == nullptr) return nullptr;                                 \
    }                                                                       \
    return ret;                                                             \
}
/// @brief Tokenizes string into nodes
/// @param str String to tokenize
/// @param i Current position in the string
/// @return Tokenized string
Node* TokenizeIndex(const std::string str, size_t& i) {
    Node* ret = TokenizeData(str, i);
    SkipWhiteSpace(str, i);
    if (str[i] == '[') {
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
Node* TokenizeFactorial(const std::string str, size_t& i) {
    Node* ret = TokenizeIndex(str, i);
    if (str[i] == '!') ret = new Node(GetType(str, i), "", ret);
    return ret;
}
TokenizeLayer(Exponentiation, str[i] == '^' || str[i] == '$', Factorial)
TokenizeLayer(Multiplication, str[i] == '*' || str[i] == '/', Exponentiation)
TokenizeLayer(Addition, str[i] == '+' || str[i] == '-', Multiplication)
TokenizeLayer(Equality, str[i] == '=', Addition)
/// @brief Tokenizes string into nodes
/// @param str String to tokenize
/// @param i Current position in the string
/// @return Tokenized string
Node* TokenizeInternal(const std::string str, size_t& i) {
    return TokenizeEquality(str, i);
}
Node* Tokenize(const std::string str) {
    size_t i = 0;
    Node* ret = TokenizeInternal(str, i);
    while (i < str.size() && ret != nullptr) ret = new Node(Node::Type::Comma, "", ret, TokenizeInternal(str, i));
    return ret;
}