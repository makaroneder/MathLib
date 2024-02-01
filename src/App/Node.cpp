#include "Node.hpp"
#include <iostream>

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
void Node::Print(std::string padding) {
    std::cout << padding << "Type: " << typeStr[(size_t)type] << "\n";
    if (value != "") std::cout << padding << "Value: " << value << "\n";
    if (left != nullptr) {
        std::cout << padding << "Left:\n";
        left->Print(padding + "\t");
    }
    if (right != nullptr) {
        std::cout << padding << "Right:\n";
        right->Print(padding + "\t");
    }
}