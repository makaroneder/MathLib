#include "Node.hpp"
#include "Host.hpp"
#include "Tokenizer.hpp"
#include "../String.hpp"

namespace MathLib {
    Array<const Node*> CommaToArray(const Node* node) {
        Array<const Node*> ret;
        if (node->type != Node::Type::Comma) return ret.Add(node) ? ret : Array<const Node*>();
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
    Node* ArrayToComma(const Sequence<Node*>& array) {
        Node* ret = array.At(0);
        size_t i = 1;
        while (i < array.GetSize() && ret) ret = new Node(Node::Type::Comma, ""_M, ret, array.At(i++));
        return ret;
    }
    Node* ReplaceVariable(const Node* node, const Sequence<char>& name, const Node* replacement) {
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
    bool ContainsVariable(const Node* node, const Sequence<char>& name) {
        if (node->type == Node::Type::Variable && node->value == name) return true;
        if (node->left) return ContainsVariable(node->left, name);
        if (node->right) return ContainsVariable(node->right, name);
        return false;
    }
    Node::Node(Type type, const Sequence<char>& value, Node* left, Node* right) : type(type), value(CollectionToString(value)), left(left), right(right) {}
    Node::~Node(void) {
        if (left != nullptr) delete left;
        if (right != nullptr) delete right;
    }
    Node* Node::Recreate(void) const {
        return new Node(type, value, !left ? nullptr : left->Recreate(), !right ? nullptr : right->Recreate());
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
            if ((i + 3) < value.GetSize() && value[i] == 'n' && value[i + 1] == 'a' && value[i + 2] == 'n') {
                i += 3;
                real += "nan";
            }
            else if ((i + 3) < value.GetSize() && value[i] == 'i' && value[i + 1] == 'n' && value[i + 2] == 'f') {
                i += 3;
                real += "inf";
            }
            else while (i < value.GetSize() && (IsDigit(value[i]) || value[i] == '.')) real += value[i++];
            SkipWhiteSpace(value, i);
            if (i >= value.GetSize())
                return ret.Add(complex_t(StringToNumber(real), 0)) ? ret : Array<complex_t>();
            else if (value[i] != '+') {
                if (value[i] == 'i')
                    return ret.Add(complex_t(0, real.IsEmpty() ? 1 : (real == "-" ? -1 : StringToNumber(real)))) ? ret : Array<complex_t>();
                else return ret.Add(complex_t(StringToNumber(real), 0)) ? ret : Array<complex_t>();
            }
            i++;
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
            return ret.Add(complex_t(StringToNumber(real), StringToNumber(imag))) ? ret : Array<complex_t>();
        }
        if (type == Type::Array) {
            const Array<const Node*> values = CommaToArray(left);
            for (const Node* const& value : values) ret += value->ToNumber();
            return ret;
        }
        return Array<complex_t>();
    }
    Array<const Node*> Node::ToArray(void) const {
        return type == Type::Array ? CommaToArray(left) : Array<const Node*>();
    }
    String Node::ToString(const Sequence<char>& padding) const {
        switch (type) {
            case Type::Function: {
                String ret = CollectionToString(padding) + value + '(';
                const Array<const Node*> params = CommaToArray(left);
                for (size_t i = 0; i < params.GetSize(); i++)
                    ret += params[i]->ToString() + (i + 1 == params.GetSize() ? ")" : ", ");
                if (right) ret += String(" : ") + right->value;
                return ret;
            }
            case Type::Variable: return CollectionToString(padding) + value + (left ? (String(" : ") + left->value) : "");
            case Type::String: return CollectionToString(padding) + '"' + value + '"';
            case Type::Constant: {
                const complex_t val = ToNumber().At(0);
                const String r = Formatter<num_t>::ToString(val.GetReal());
                const String i = Formatter<num_t>::ToString(val.GetImaginary());
                if (!val.GetReal() && !val.GetImaginary()) return CollectionToString(padding) + '0';
                else if (!val.GetReal()) return CollectionToString(padding) + (val.GetImaginary() == 1 ? "" : i) + 'i';
                else if (!val.GetImaginary()) return CollectionToString(padding) + r;
                else return CollectionToString(padding) + '(' + r + " + " + (val.GetImaginary() == 1 ? "" : i) + "i)";
            }
            case Type::Index: return CollectionToString(padding) + left->ToString() + '[' + right->ToString() + ']';
            case Type::Comma: {
                String ret = "List:\n";
                const Array<const Node*> values = CommaToArray(this);
                for (const Node* const& value : values) ret += value->ToString(CollectionToString(padding) + '\t') + '\n';
                return ret;
            }
            case Type::Array: {
                String ret = CollectionToString(padding) + "{ ";
                const Array<const Node*> params = CommaToArray(left);
                for (size_t i = 0; i < params.GetSize(); i++)
                    ret += params[i]->ToString() + (i + 1 == params.GetSize() ? " }" : ", ");
                return ret;
            }
            case Type::Program: {
                String ret = CollectionToString(padding) + '[' + (value == "1" ? "!" : "") + '\n';
                const Array<const Node*> params = CommaToArray(left);
                for (size_t i = 0; i < params.GetSize(); i++)
                    ret += params[i]->ToString(CollectionToString(padding) + '\t') + (i + 1 == params.GetSize() ? ('\n'_M + padding + ']') : '\n');
                return ret;
            }
            case Type::Equal:
            case Type::DynamicEqual: {
                const String tmp = right->ToString(padding);
                return CollectionToString(padding) + left->ToString() + (type == Type::Equal ? " = " : " := ") + SubString(tmp, padding.GetSize(), tmp.GetSize() - padding.GetSize());
            }
            case Type::Add: return CollectionToString(padding) + '(' + left->ToString() + " + " + right->ToString() + ')';
            case Type::Sub: return CollectionToString(padding) + '(' + left->ToString() + " - " + right->ToString() + ')';
            case Type::Mul: return CollectionToString(padding) + '(' + left->ToString() + " * " + right->ToString() + ')';
            case Type::Div: return CollectionToString(padding) + '(' + left->ToString() + " / " + right->ToString() + ')';
            case Type::Pow: return CollectionToString(padding) + '(' + left->ToString() + " ^ " + right->ToString() + ')';
            case Type::Root: return CollectionToString(padding) + '(' + left->ToString() + " $ " + right->ToString() + ')';
            case Type::Factorial: return CollectionToString(padding) + left->ToString() + '!';
            case Type::Absolute: return CollectionToString(padding) + '|' + left->ToString() + '|';
            case Type::LogicalEqual: return CollectionToString(padding) + '(' + left->ToString() + " == " + right->ToString() + ')';
            case Type::LogicalNotEqual: return CollectionToString(padding) + '(' + left->ToString() + " != " + right->ToString() + ')';
            case Type::LessThan: return CollectionToString(padding) + '(' + left->ToString() + " < " + right->ToString() + ')';
            case Type::GreaterThan: return CollectionToString(padding) + '(' + left->ToString() + " > " + right->ToString() + ')';
            case Type::LessThanEqual: return CollectionToString(padding) + '(' + left->ToString() + " <= " + right->ToString() + ')';
            case Type::GreaterThanEqual: return CollectionToString(padding) + '(' + left->ToString() + " >= " + right->ToString() + ')';
            case Type::Struct: {
                const Array<const Node*> fields = CommaToArray(left);
                String ret = CollectionToString(padding) + "{\n";
                for (size_t i = 0; i < fields.GetSize(); i += 2)
                    ret += fields.At(i)->ToString(CollectionToString(padding) + '\t') + " = " + fields.At(i + 1)->ToString() + '\n';
                return ret + padding + '}';
            }
            case Type::Keyword: {
                if (value == "if") {
                    const Array<const Node*> params = CommaToArray(left);
                    return CollectionToString(padding) + "if (" + params.At(0)->ToString() + ")\n" + params.At(1)->ToString(CollectionToString(padding) + '\t') + ((params.GetSize() == 3) ? String('\n') + padding + "else\n" + params.At(2)->ToString(CollectionToString(padding) + '\t') : "");
                }
                if (value == "while") {
                    const Array<const Node*> params = CommaToArray(left);
                    return CollectionToString(padding) + "while (" + params.At(0)->ToString() + ")\n" + params.At(1)->ToString(CollectionToString(padding) + '\t');
                }
                if (value == "return") return CollectionToString(padding) + "return " + left->ToString();
                if (value == "integral") {
                    const Array<const Node*> params = CommaToArray(left);
                    return CollectionToString(padding) + "integral(" + params.At(0)->ToString() + " d" + params.At(1)->ToString() + ')';
                }
                if (value == "summation") {
                    const Array<const Node*> params = CommaToArray(left);
                    return CollectionToString(padding) + "summation(" + params.At(0)->ToString() + " = " + params.At(1)->ToString() + ", " + params.At(0)->ToString() + " <= " + params.At(2)->ToString() + ", " + params.At(3)->ToString()  + ')';
                }
                if (value == "product") {
                    const Array<const Node*> params = CommaToArray(left);
                    return CollectionToString(padding) + "product(" + params.At(0)->ToString() + " = " + params.At(1)->ToString() + ", " + params.At(0)->ToString() + " <= " + params.At(2)->ToString() + ", " + params.At(3)->ToString()  + ')';
                }
                if (value == "MakeStruct") {
                    const Array<const Node*> fields = CommaToArray(left);
                    String ret = CollectionToString(padding) + "{\n";
                    for (const Node* const& field : fields) ret += field->ToString(CollectionToString(padding) + '\t') + '\n';
                    return ret + padding + '}';
                }
                if (value == "GetField") {
                    const Array<const Node*> params = CommaToArray(left);
                    return CollectionToString(padding) + "SetField(" + params.At(0)->ToString() + ", " + params.At(1)->ToString() + ", " + params.At(2)->ToString() + ')';
                }
                if (value == "SetField") {
                    const Array<const Node*> params = CommaToArray(left);
                    return CollectionToString(padding) + "SetField(" + params.At(0)->ToString() + ", " + params.At(1)->ToString() + ')';
                }
                return CollectionToString(padding) + value;
            }
            default: return CollectionToString(padding) + "nan";
        }
    }
}