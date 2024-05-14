#include "Node.hpp"
#include <sstream>

std::vector<const Node*> CommaToArray(const Node* node) {
    if (node->type != Node::Type::Comma) return { node, };
    else {
        std::vector<const Node*> ret;
        if (node->left != nullptr) {
            std::vector<const Node*> tmp = CommaToArray(node->left);
            for (const Node*& i : tmp) ret.push_back(i);
        }
        if (node->right != nullptr) {
            std::vector<const Node*> tmp = CommaToArray(node->right);
            for (const Node*& i : tmp) ret.push_back(i);
        }
        return ret;
    }
}
Node* ArrayToComma(std::vector<Node*> array) {
    Node* ret = array.at(0);
    size_t i = 1;
    while (i < array.size()) ret = new Node(Node::Type::Comma, "", ret, array.at(i++));
    return ret;
}
Node* ReplaceNode(const Node* node, std::function<Node*(const Node*)> f) {
    Node* ret = f(node);
    if (node->left != nullptr) ret->left = ReplaceNode(node->left, f);
    if (node->right != nullptr) ret->right = ReplaceNode(node->right, f);
    return ret;
}
bool ContainsNode(const Node* node, std::function<bool(const Node*)> f) {
    if (f(node)) return true;
    if (node->left != nullptr) if (ContainsNode(node->left, f)) return true;
    if (node->right != nullptr) if (ContainsNode(node->right, f)) return true;
    return false;
}
Node::Node(Type t, std::string val, Node* l, Node* r) : type(t), value(val), left(l), right(r) {}
Node::~Node(void) {
    if (left != nullptr) delete left;
    if (right != nullptr) delete right;
}
bool Node::IsConstant(void) const {
    return type == Type::Constant || type == Type::ComplexConstant;
}
Node* Node::Recreate(void) const {
    return new Node(type, value, left == nullptr ? nullptr : left->Recreate(), right == nullptr ? nullptr : right->Recreate());
}
std::vector<complex_t> Node::ToNumber(void) const {
    if (type == Type::Constant) return { complex_t(std::stold(value), 0), };
    else if (type == Type::ComplexConstant) {
        std::istringstream iss = std::istringstream(value);
        complex_t ret;
        iss >> ret;
        return { ret, };
    }
    else if (type == Type::Array) {
        std::vector<const Node*> values = CommaToArray(left);
        std::vector<complex_t> ret;
        for (const Node*& val : values) {
            std::vector<complex_t> tmp = val->ToNumber();
            if (tmp.empty()) return { };
            for (complex_t& num : tmp) ret.push_back(num);
        }
        return ret;
    }
    else return { };
}
std::string Node::ToString(std::string padding) const {
    switch (type) {
        case Type::Function: {
            std::string ret = padding + value + '(';
            std::vector<const Node*> params = CommaToArray(left);
            for (size_t i = 0; i < params.size(); i++)
                ret += params[i]->ToString() + (i + 1 == params.size() ? ")" : ", ");
            if (right != nullptr) {
                std::vector<const Node*> sets = CommaToArray(right);
                ret += " : " + sets.at(0)->ToString() + " -> " + sets.at(1)->ToString();
            }
            return ret;
        }
        case Type::Constant:
        case Type::Variable: return padding + value;
        case Type::String: return padding + '"' + value + '"';
        case Type::ComplexConstant: {
            const complex_t val = ToNumber().at(0);
            const std::string r = std::to_string(val.real());
            const std::string i = std::to_string(val.imag());
            if (val.real() == 0) return padding + (val.imag() == 1 ? "" : i) + 'i';
            else if (val.imag() == 0) return padding + r;
            return padding + '(' + r + " + " + (val.imag() == 1 ? "" : i) + "i)";
        }
        case Type::Index: {
            return padding + left->ToString() + '[' + right->ToString() + ']';
        }
        case Type::Comma: {
            std::string ret = "List:\n";
            std::vector<const Node*> values = CommaToArray(this);
            for (const Node*& val : values) ret += val->ToString(padding + '\t') + '\n';
            return ret;
        }
        case Type::Array: {
            std::string ret = padding + '{';
            std::vector<const Node*> params = CommaToArray(left);
            for (size_t i = 0; i < params.size(); i++)
                ret += params[i]->ToString() + (i + 1 == params.size() ? "}" : ", ");
            return ret;
        }
        case Type::Equal: return padding + left->ToString() + " = " + right->ToString();
        case Type::Add: return padding + '(' + left->ToString() + " + " + right->ToString() + ')';
        case Type::Sub: return padding + '(' + left->ToString() + " - " + right->ToString() + ')';
        case Type::Mul: return padding + '(' + left->ToString() + " * " + right->ToString() + ')';
        case Type::Div: return padding + '(' + left->ToString() + " / " + right->ToString() + ')';
        case Type::Pow: return padding + '(' + left->ToString() + " ^ " + right->ToString() + ')';
        case Type::Root: return padding + '(' + left->ToString() + " $ " + right->ToString() + ')';
        case Type::Factorial: return padding + left->ToString() + '!';
        case Type::Absolute: return padding + '|' + left->ToString() + '|';
        case Type::Integral: {
            std::vector<const Node*> params = CommaToArray(left);
            return padding + "integral(" + params.at(0)->ToString() + " d" + params.at(1)->ToString() + ')';
        }
        case Type::Summation: {
            std::vector<const Node*> params = CommaToArray(left);
            return padding + "summation(" + params.at(0)->ToString() + " = " + params.at(1)->ToString() + ", " + params.at(0)->ToString() + " <= " + params.at(2)->ToString() + ", " + params.at(3)->ToString()  + ')';
        }
        case Type::Product: {
            std::vector<const Node*> params = CommaToArray(left);
            return padding + "product(" + params.at(0)->ToString() + " = " + params.at(1)->ToString() + ", " + params.at(0)->ToString() + " <= " + params.at(2)->ToString() + ", " + params.at(3)->ToString()  + ')';
        }
        default: return padding + "nan";
    }
}