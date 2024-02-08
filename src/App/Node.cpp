#include "Node.hpp"

std::vector<Node*> CommaToArray(Node* node) {
    if (node->type != Node::Type::Comma) return { node, };
    else {
        std::vector<Node*> ret;
        if (node->left != nullptr) {
            std::vector<Node*> tmp = CommaToArray(node->left);
            for (Node*& i : tmp) ret.push_back(i);
        }
        if (node->right != nullptr) {
            std::vector<Node*> tmp = CommaToArray(node->right);
            for (Node*& i : tmp) ret.push_back(i);
        }
        return ret;
    }
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
Node* Node::Recreate(void) {
    return new Node(type, value, left == nullptr ? nullptr : left->Recreate(), right == nullptr ? nullptr : right->Recreate());
}
std::string Node::ToString(std::string padding) {
    switch (type) {
        case Node::Type::Function: {
            std::string ret = padding + value + '(';
            std::vector<Node*> params = CommaToArray(left);
            for (size_t i = 0; i < params.size(); i++)
                ret += params[i]->ToString() + (i + 1 == params.size() ? ")" : ", ");
            return ret;
        }
        case Node::Type::Constant:
        case Node::Type::Variable: return padding + value;
        case Node::Type::Comma: {
            std::string ret = "List:\n";
            std::vector<Node*> values = CommaToArray(this);
            for (Node*& val : values) ret += val->ToString(padding + '\t') + '\n';
            return ret;
        }
        case Node::Type::Equal: return padding + left->ToString() + " = " + right->ToString();
        case Node::Type::Add: return padding + left->ToString() + " + " + right->ToString();
        case Node::Type::Sub: return padding + left->ToString() + " - " + right->ToString();
        case Node::Type::Mul: return padding + left->ToString() + " * " + right->ToString();
        case Node::Type::Div: return padding + left->ToString() + " / " + right->ToString();
        case Node::Type::Pow: return padding + left->ToString() + " ^ " + right->ToString();
        case Node::Type::Root: return padding + left->ToString() + " $ " + right->ToString();
        case Node::Type::Factorial: return padding + left->ToString() + '!';
        case Node::Type::Absolute: return padding + '|' + left->ToString() + '|';
        default: return "nan";
    }
}