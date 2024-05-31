#include "Node.hpp"
#include "Host.hpp"
#include "Tokenizer.hpp"

Array<const Node*> CommaToArray(const Node* node) {
    Array<const Node*> ret;
    if (node->type != Node::Type::Comma) {
        if (!ret.Add(node)) return Array<const Node*>();
        return ret;
    }
    else {
        if (node->left != nullptr) {
            Array<const Node*> tmp = CommaToArray(node->left);
            for (size_t i = 0; i < tmp.GetSize(); i++)
                if (!ret.Add(tmp.At(i))) return Array<const Node*>();
        }
        if (node->right != nullptr) {
            Array<const Node*> tmp = CommaToArray(node->right);
            for (size_t i = 0; i < tmp.GetSize(); i++)
                if (!ret.Add(tmp.At(i))) return Array<const Node*>();
        }
        return ret;
    }
}
Node* ArrayToComma(Array<Node*> array) {
    Node* ret = array.At(0);
    size_t i = 1;
    while (i < array.GetSize()) ret = new Node(Node::Type::Comma, "", ret, array.At(i++));
    return ret;
}
Node* ReplaceVariable(const Node* node, String name, const Node* replacement) {
    if (node->type == Node::Type::Variable && node->value == name) return replacement->Recreate();
    Node* ret = node->Recreate();
    if (ret->left) ret->left = ReplaceVariable(ret->left, name, replacement);
    if (ret->right) ret->right = ReplaceVariable(ret->right, name, replacement);
    return ret;
}
bool ContainsVariable(const Node* node, String name) {
    if (node->type == Node::Type::Variable && node->value == name) return true;
    if (node->left) return ContainsVariable(node->left, name);
    if (node->right) return ContainsVariable(node->right, name);
    return false;
}
Node::Node(Type t, String val, Node* l, Node* r) : type(t), value(val), left(l), right(r) {}
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
Array<complex_t> Node::ToNumber(void) const {
    Array<complex_t> ret;
    if (type == Type::Constant) {
        ret.Add(complex_t(StringToNumber(value), 0));
        return ret;
    }
    else if (type == Type::ComplexConstant) {
        size_t i = 0;
        if (value[i++] != '(') {
            ret.Add(complex_t(StringToNumber(value), 0));
            return ret;
        }
        SkipWhiteSpace(value, i);
        String real;
        while (i < value.GetSize() && (IsDigit(value[i]) || value[i] == '.' || value[i] == '-')) real += value[i++];
        SkipWhiteSpace(value, i);
        if (value[i++] != ',') return ret;
        SkipWhiteSpace(value, i);
        String imag;
        while (i < value.GetSize() && (IsDigit(value[i]) || value[i] == '.' || value[i] == '-')) imag += value[i++];
        SkipWhiteSpace(value, i);
        if (value[i++] != ')') return ret;
        ret.Add(complex_t(StringToNumber(real), StringToNumber(imag)));
        return ret;
    }
    else if (type == Type::Array) {
        Array<const Node*> values = CommaToArray(left);
        for (size_t i = 0; i < values.GetSize(); i++){
            Array<complex_t> tmp = values.At(i)->ToNumber();
            if (tmp.IsEmpty()) return Array<complex_t>();
            for (size_t j = 0; j < tmp.GetSize(); j++) ret.Add(tmp.At(j));
        }
        return ret;
    }
    else return Array<complex_t>();
}
String Node::ToString(String padding) const {
    switch (type) {
        case Type::Function: {
            String ret = padding + value + '(';
            Array<const Node*> params = CommaToArray(left);
            for (size_t i = 0; i < params.GetSize(); i++)
                ret += params[i]->ToString() + (i + 1 == params.GetSize() ? ")" : ", ");
            if (right != nullptr) {
                Array<const Node*> sets = CommaToArray(right);
                ret += String(" : ") + sets.At(0)->ToString() + " -> " + sets.At(1)->ToString();
            }
            return ret;
        }
        case Type::Constant:
        case Type::Variable: return padding + value;
        case Type::String: return padding + '"' + value + '"';
        case Type::ComplexConstant: {
            const complex_t val = ToNumber().At(0);
            const String r = std::to_string(val.real());
            const String i = std::to_string(val.imag());
            if (val.real() == 0) return padding + (val.imag() == 1 ? "" : i) + 'i';
            else if (val.imag() == 0) return padding + r;
            return padding + '(' + r + " + " + (val.imag() == 1 ? "" : i) + "i)";
        }
        case Type::Index: {
            return padding + left->ToString() + '[' + right->ToString() + ']';
        }
        case Type::Comma: {
            String ret = "List:\n";
            Array<const Node*> values = CommaToArray(this);
            for (size_t i = 0; i < values.GetSize(); i++) ret += values.At(i)->ToString(padding + '\t') + '\n';
            return ret;
        }
        case Type::Array: {
            String ret = padding + '{';
            Array<const Node*> params = CommaToArray(left);
            for (size_t i = 0; i < params.GetSize(); i++)
                ret += params[i]->ToString() + (i + 1 == params.GetSize() ? "}" : ", ");
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
            Array<const Node*> params = CommaToArray(left);
            return padding + "integral(" + params.At(0)->ToString() + " d" + params.At(1)->ToString() + ')';
        }
        case Type::Summation: {
            Array<const Node*> params = CommaToArray(left);
            return padding + "summation(" + params.At(0)->ToString() + " = " + params.At(1)->ToString() + ", " + params.At(0)->ToString() + " <= " + params.At(2)->ToString() + ", " + params.At(3)->ToString()  + ')';
        }
        case Type::Product: {
            Array<const Node*> params = CommaToArray(left);
            return padding + "product(" + params.At(0)->ToString() + " = " + params.At(1)->ToString() + ", " + params.At(0)->ToString() + " <= " + params.At(2)->ToString() + ", " + params.At(3)->ToString()  + ')';
        }
        default: return padding + "nan";
    }
}