#include "Node.hpp"

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
Node::Node(Type t, std::string val, Node* l, Node* r) {
    type = t;
    value = val;
    left = l;
    right = r;
}
Node::~Node(void) {
    if (left != nullptr) delete left;
    if (right != nullptr) delete right;
}
Node* Node::Recreate(void) const {
    return new Node(type, value, left == nullptr ? nullptr : left->Recreate(), right == nullptr ? nullptr : right->Recreate());
}
std::vector<num_t> Node::ToNumber(void) const {
    if (type == Node::Type::Constant) return { std::stold(value), };
    else if (type == Node::Type::Array) {
        std::vector<const Node*> values = CommaToArray(left);
        std::vector<num_t> ret;
        for (const Node*& val : values) {
            std::vector<num_t> tmp = val->ToNumber();
            if (tmp.empty()) return { };
            for (num_t& num : tmp) ret.push_back(num);
        }
        return ret;
    }
    else return { };
}
std::string Node::ToString(std::string padding) const {
    switch (type) {
        case Node::Type::Function: {
            std::string ret = padding + value + '(';
            std::vector<const Node*> params = CommaToArray(left);
            for (size_t i = 0; i < params.size(); i++)
                ret += params[i]->ToString() + (i + 1 == params.size() ? ")" : ", ");
            return ret;
        }
        case Node::Type::Constant:
        case Node::Type::Variable: return padding + value;
        case Node::Type::Index: {
            return padding + left->ToString() + '[' + right->ToString() + ']';
        }
        case Node::Type::Comma: {
            std::string ret = "List:\n";
            std::vector<const Node*> values = CommaToArray(this);
            for (const Node*& val : values) ret += val->ToString(padding + '\t') + '\n';
            return ret;
        }
        case Node::Type::Array: {
            std::string ret = padding + '{';
            std::vector<const Node*> params = CommaToArray(left);
            for (size_t i = 0; i < params.size(); i++)
                ret += params[i]->ToString() + (i + 1 == params.size() ? "}" : ", ");
            return ret;
        }
        case Node::Type::Equal: return padding + left->ToString() + " = " + right->ToString();
        case Node::Type::Add: return padding + '(' + left->ToString() + " + " + right->ToString() + ')';
        case Node::Type::Sub: return padding + '(' + left->ToString() + " - " + right->ToString() + ')';
        case Node::Type::Mul: return padding + '(' + left->ToString() + " * " + right->ToString() + ')';
        case Node::Type::Div: return padding + '(' + left->ToString() + " / " + right->ToString() + ')';
        case Node::Type::Pow: return padding + '(' + left->ToString() + " ^ " + right->ToString() + ')';
        case Node::Type::Root: return padding + '(' + left->ToString() + " $ " + right->ToString() + ')';
        case Node::Type::Factorial: return padding + left->ToString() + '!';
        case Node::Type::Absolute: return padding + '|' + left->ToString() + '|';
        case Node::Type::Integral: {
            std::vector<const Node*> params = CommaToArray(left);
            return padding + "integral(" + params.at(0)->ToString() + " d" + params.at(1)->ToString() + ')';
        }
        case Node::Type::Summation: {
            std::vector<const Node*> params = CommaToArray(left);
            return padding + "summation(" + params.at(0)->ToString() + " = " + params.at(1)->ToString() + ", " + params.at(0)->ToString() + " <= " + params.at(2)->ToString() + ", " + params.at(3)->ToString()  + ')';
        }
        case Node::Type::Product: {
            std::vector<const Node*> params = CommaToArray(left);
            return padding + "product(" + params.at(0)->ToString() + " = " + params.at(1)->ToString() + ", " + params.at(0)->ToString() + " <= " + params.at(2)->ToString() + ", " + params.at(3)->ToString()  + ')';
        }
        default: return "nan";
    }
}