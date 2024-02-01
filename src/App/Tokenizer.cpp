#include "Tokenizer.hpp"
#include <string.h>

/// @brief Skips whitespace characters
/// @param str String to skip
/// @param i Current position in the string
void SkipWhiteSpace(const std::string str, size_t& i) {
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\r') i++;
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
        while (i < str.size() && (isdigit(str[i]) || str[i] == '.')) ret += str[i++];
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
            return new Node(Node::Type::Function, name, ret);
        }
        else return new Node(Node::Type::Variable, name);
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
Node* TokenizeFactorial(const std::string str, size_t& i) {
    Node* ret = TokenizeData(str, i);
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