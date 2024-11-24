#include "Node.hpp"
#include "Host.hpp"
#include "Tokenizer.hpp"

namespace MathLib {
    Array<const Node*> CommaToArray(const Node* node) {
        Array<const Node*> ret;
        if (node->type != Node::Type::Comma) {
            if (!ret.Add(node)) return Array<const Node*>();
            return ret;
        }
        else {
            if (node->left != nullptr) {
                const Array<const Node*> tmp = CommaToArray(node->left);
                for (const Node* const& x : tmp)
                    if (!ret.Add(x)) return Array<const Node*>();
            }
            if (node->right != nullptr) {
                const Array<const Node*> tmp = CommaToArray(node->right);
                for (const Node* const& x : tmp)
                    if (!ret.Add(x)) return Array<const Node*>();
            }
            return ret;
        }
    }
    Node* ArrayToComma(const Array<Node*>& array) {
        Node* ret = array.At(0);
        size_t i = 1;
        while (i < array.GetSize()) ret = new Node(Node::Type::Comma, "", ret, array.At(i++));
        return ret;
    }
    Node* ReplaceVariable(const Node* node, const String& name, const Node* replacement) {
        if (node->type == Node::Type::Variable && node->value == name) return replacement->Recreate();
        Node* ret = node->Recreate();
        if (ret->left) {
            Node* tmp = ReplaceVariable(ret->left, name, replacement);
            delete ret->left;
            ret->left = tmp;
        }
        if (ret->right) {
            Node* tmp = ReplaceVariable(ret->right, name, replacement);
            delete ret->right;
            ret->right = tmp;
        }
        return ret;
    }
    bool ContainsVariable(const Node* node, const String& name) {
        if (node->type == Node::Type::Variable && node->value == name) return true;
        if (node->left) return ContainsVariable(node->left, name);
        if (node->right) return ContainsVariable(node->right, name);
        return false;
    }
    Node::Node(Type type, const String& value, Node* left, Node* right) : type(type), value(value), left(left), right(right) {}
    Node::~Node(void) {
        if (left != nullptr) delete left;
        if (right != nullptr) delete right;
    }
    Node* Node::Recreate(void) const {
        return new Node(type, value, left == nullptr ? nullptr : left->Recreate(), right == nullptr ? nullptr : right->Recreate());
    }
    Array<complex_t> Node::ToNumber(void) const {
        Array<complex_t> ret;
        if (type == Type::Constant) {
            size_t i = 0;
            String real;
            if (value[i] == '-') {
                real += '-';
                i++;
            }
            if (value[i] == 'n' && value[i + 1] == 'a' && value[i + 2] == 'n') {
                i += 3;
                real += "nan";
            }
            else if (value[i] == 'i' && value[i + 1] == 'n' && value[i + 2] == 'f') {
                i += 3;
                real += "inf";
            }
            else while (i < value.GetSize() && (IsDigit(value[i]) || value[i] == '.')) real += value[i++];
            SkipWhiteSpace(value, i);
            if (i >= value.GetSize() || value[i++] != '+') {
                ret.Add(complex_t(StringToNumber(real), 0));
                return ret;
            }
            SkipWhiteSpace(value, i);
            String imag;
            if (value[i] == '-') {
                imag += '-';
                i++;
            }
            if (value[i] == 'n' && value[i + 1] == 'a' && value[i + 2] == 'n') {
                i += 3;
                imag += "nan";
            }
            else if (value[i] == 'i' && value[i + 1] == 'n' && value[i + 2] == 'f') {
                i += 3;
                imag += "inf";
            }
            else while (i < value.GetSize() && (IsDigit(value[i]) || value[i] == '.')) imag += value[i++];
            SkipWhiteSpace(value, i);
            if (value[i++] != 'i') return ret;
            ret.Add(complex_t(StringToNumber(real), StringToNumber(imag)));
            return ret;
        }
        else if (type == Type::Array) {
            const Array<const Node*> values = CommaToArray(left);
            for (const Node* const& value : values) {
                const Array<complex_t> tmp = value->ToNumber();
                if (tmp.IsEmpty()) return Array<complex_t>();
                for (const complex_t& x : tmp) ret.Add(x);
            }
            return ret;
        }
        else return Array<complex_t>();
    }
    Array<const Node*> Node::ToArray(void) const {
        return type == Type::Array ? CommaToArray(left) : Array<const Node*>();
    }
    String Node::ToString(const String& padding) const {
        switch (type) {
            case Type::Function: {
                String ret = padding + value + '(';
                const Array<const Node*> params = CommaToArray(left);
                for (size_t i = 0; i < params.GetSize(); i++)
                    ret += params[i]->ToString() + (i + 1 == params.GetSize() ? ")" : ", ");
                if (right != nullptr) ret += String(" : ") + right->value;
                return ret;
            }
            case Type::Variable: return padding + value + (left ? (String(" : ") + left->value) : "");
            case Type::String: return padding + '"' + value + '"';
            case Type::Constant: {
                const complex_t val = ToNumber().At(0);
                const String r = MathLib::ToString(val.GetReal());
                const String i = MathLib::ToString(val.GetImaginary());
                if (!val.GetReal() && !val.GetImaginary()) return padding + '0';
                else if (!val.GetReal()) return padding + (val.GetImaginary() == 1 ? "" : i) + 'i';
                else if (!val.GetImaginary()) return padding + r;
                return padding + '(' + r + " + " + (val.GetImaginary() == 1 ? "" : i) + "i)";
            }
            case Type::Index: {
                return padding + left->ToString() + '[' + right->ToString() + ']';
            }
            case Type::Comma: {
                String ret = "List:\n";
                const Array<const Node*> values = CommaToArray(this);
                for (const Node* const& value : values) ret += value->ToString(padding + '\t') + '\n';
                return ret;
            }
            case Type::Array: {
                String ret = padding + "{ ";
                const Array<const Node*> params = CommaToArray(left);
                for (size_t i = 0; i < params.GetSize(); i++)
                    ret += params[i]->ToString() + (i + 1 == params.GetSize() ? " }" : ", ");
                return ret;
            }
            case Type::Program: {
                String ret = padding + '[' + (value == "1" ? "!" : "") + '\n';
                const Array<const Node*> params = CommaToArray(left);
                for (size_t i = 0; i < params.GetSize(); i++)
                    ret += params[i]->ToString(padding + '\t') + (i + 1 == params.GetSize() ? String('\n') + padding + ']' : '\n');
                return ret;
            }
            case Type::Equal: return padding + left->ToString() + " = " + right->ToString();
            case Type::DynamicEqual: return padding + left->ToString() + " := " + right->ToString();
            case Type::Add: return padding + '(' + left->ToString() + " + " + right->ToString() + ')';
            case Type::Sub: return padding + '(' + left->ToString() + " - " + right->ToString() + ')';
            case Type::Mul: return padding + '(' + left->ToString() + " * " + right->ToString() + ')';
            case Type::Div: return padding + '(' + left->ToString() + " / " + right->ToString() + ')';
            case Type::Pow: return padding + '(' + left->ToString() + " ^ " + right->ToString() + ')';
            case Type::Root: return padding + '(' + left->ToString() + " $ " + right->ToString() + ')';
            case Type::Factorial: return padding + left->ToString() + '!';
            case Type::Absolute: return padding + '|' + left->ToString() + '|';
            case Type::LogicalEqual: return padding + '(' + left->ToString() + " == " + right->ToString() + ')';
            case Type::LogicalNotEqual: return padding + '(' + left->ToString() + " != " + right->ToString() + ')';
            case Type::LessThan: return padding + '(' + left->ToString() + " < " + right->ToString() + ')';
            case Type::GreaterThan: return padding + '(' + left->ToString() + " > " + right->ToString() + ')';
            case Type::LessThanEqual: return padding + '(' + left->ToString() + " <= " + right->ToString() + ')';
            case Type::GreaterThanEqual: return padding + '(' + left->ToString() + " >= " + right->ToString() + ')';
            case Type::Keyword: {
                if (value == "if") {
                    const Array<const Node*> params = CommaToArray(left);
                    return padding + "if (" + params.At(0)->ToString() + ")\n" + params.At(1)->ToString(padding + '\t') + ((params.GetSize() == 3) ? String('\n') + padding + "else\n" + params.At(2)->ToString(padding + '\t') : "");
                }
                else if (value == "while") {
                    const Array<const Node*> params = CommaToArray(left);
                    return padding + "while (" + params.At(0)->ToString() + ")\n" + params.At(1)->ToString(padding + '\t');
                }
                else if (value == "return") return padding + "return " + left->ToString();
                else if (value == "integral") {
                    const Array<const Node*> params = CommaToArray(left);
                    return padding + "integral(" + params.At(0)->ToString() + " d" + params.At(1)->ToString() + ')';
                }
                else if (value == "summation") {
                    const Array<const Node*> params = CommaToArray(left);
                    return padding + "summation(" + params.At(0)->ToString() + " = " + params.At(1)->ToString() + ", " + params.At(0)->ToString() + " <= " + params.At(2)->ToString() + ", " + params.At(3)->ToString()  + ')';
                }
                else if (value == "product") {
                    const Array<const Node*> params = CommaToArray(left);
                    return padding + "product(" + params.At(0)->ToString() + " = " + params.At(1)->ToString() + ", " + params.At(0)->ToString() + " <= " + params.At(2)->ToString() + ", " + params.At(3)->ToString()  + ')';
                }
                else return padding + value;
            
            }
            default: return padding + "nan";
        }
    }
}