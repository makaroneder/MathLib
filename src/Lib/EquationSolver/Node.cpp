#include "Node.hpp"
#include "Host.hpp"
#include "Tokenizer.hpp"
#include "../String.hpp"

namespace MathLib {
    Array<const Node*> CommaToArray(const Node* node) {
        StartBenchmark
        Array<const Node*> ret;
        if (node->type != Node::Type::Comma) ReturnFromBenchmark(ret.Add(node) ? ret : Array<const Node*>())
        else {
            if (node->left != nullptr) {
                const Array<const Node*> tmp = CommaToArray(node->left);
                for (const Node* const& x : tmp)
                    if (!ret.Add(x)) ReturnFromBenchmark(Array<const Node*>());
            }
            if (node->right != nullptr) {
                const Array<const Node*> tmp = CommaToArray(node->right);
                for (const Node* const& x : tmp)
                    if (!ret.Add(x)) ReturnFromBenchmark(Array<const Node*>());
            }
            ReturnFromBenchmark(ret);
        }
    }
    Node* ArrayToComma(const Sequence<Node*>& array) {
        StartBenchmark
        Node* ret = array.At(0);
        size_t i = 1;
        while (i < array.GetSize() && ret) ret = new Node(Node::Type::Comma, ""_M, ret, array.At(i++));
        ReturnFromBenchmark(ret);
    }
    Node* ReplaceVariable(const Node* node, const Sequence<char>& name, const Node* replacement) {
        StartBenchmark
        if (node->type == Node::Type::Variable && node->value == name) ReturnFromBenchmark(replacement->Recreate());
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
        ReturnFromBenchmark(ret);
    }
    bool ContainsVariable(const Node* node, const Sequence<char>& name) {
        StartBenchmark
        if (node->type == Node::Type::Variable && node->value == name) ReturnFromBenchmark(true)
        if (node->left) ReturnFromBenchmark(ContainsVariable(node->left, name))
        if (node->right) ReturnFromBenchmark(ContainsVariable(node->right, name))
        ReturnFromBenchmark(false);
    }
    Node::Node(Type type, const Sequence<char>& value, Node* left, Node* right) : type(type), value(CollectionToString(value)), left(left), right(right) {
        EmptyBenchmark
    }
    Node::~Node(void) {
        StartBenchmark
        if (left != nullptr) delete left;
        if (right != nullptr) delete right;
        EndBenchmark
    }
    Node* Node::Recreate(void) const {
        StartAndReturnFromBenchmark(new Node(type, value, !left ? nullptr : left->Recreate(), !right ? nullptr : right->Recreate()));
    }
    Array<complex_t> Node::ToNumber(void) const {
        StartBenchmark
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
                ReturnFromBenchmark(ret.Add(complex_t(StringToNumber(real), 0)) ? ret : Array<complex_t>())
            else if (value[i] != '+') {
                if (value[i] == 'i')
                    ReturnFromBenchmark(ret.Add(complex_t(0, real.IsEmpty() ? 1 : (real == "-" ? -1 : StringToNumber(real)))) ? ret : Array<complex_t>())
                else ReturnFromBenchmark(ret.Add(complex_t(StringToNumber(real), 0)) ? ret : Array<complex_t>());
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
            if (value[i++] != 'i') ReturnFromBenchmark(ret);
            ReturnFromBenchmark(ret.Add(complex_t(StringToNumber(real), StringToNumber(imag))) ? ret : Array<complex_t>());
        }
        else if (type == Type::Array) {
            const Array<const Node*> values = CommaToArray(left);
            for (const Node* const& value : values) ret += value->ToNumber();
            ReturnFromBenchmark(ret);
        }
        else ReturnFromBenchmark(Array<complex_t>());
    }
    Array<const Node*> Node::ToArray(void) const {
        StartAndReturnFromBenchmark(type == Type::Array ? CommaToArray(left) : Array<const Node*>());
    }
    String Node::ToString(const Sequence<char>& padding) const {
        StartBenchmark
        switch (type) {
            case Type::Function: {
                String ret = CollectionToString(padding) + value + '(';
                const Array<const Node*> params = CommaToArray(left);
                for (size_t i = 0; i < params.GetSize(); i++)
                    ret += params[i]->ToString() + (i + 1 == params.GetSize() ? ")" : ", ");
                if (right) ret += String(" : ") + right->value;
                ReturnFromBenchmark(ret);
            }
            case Type::Variable: ReturnFromBenchmark(CollectionToString(padding) + value + (left ? (String(" : ") + left->value) : ""));
            case Type::String: ReturnFromBenchmark(CollectionToString(padding) + '"' + value + '"');
            case Type::Constant: {
                const complex_t val = ToNumber().At(0);
                const String r = MathLib::ToString(val.GetReal());
                const String i = MathLib::ToString(val.GetImaginary());
                if (!val.GetReal() && !val.GetImaginary()) ReturnFromBenchmark(CollectionToString(padding) + '0')
                else if (!val.GetReal()) ReturnFromBenchmark(CollectionToString(padding) + (val.GetImaginary() == 1 ? "" : i) + 'i')
                else if (!val.GetImaginary()) ReturnFromBenchmark(CollectionToString(padding) + r)
                else ReturnFromBenchmark(CollectionToString(padding) + '(' + r + " + " + (val.GetImaginary() == 1 ? "" : i) + "i)");
            }
            case Type::Index: ReturnFromBenchmark(CollectionToString(padding) + left->ToString() + '[' + right->ToString() + ']');
            case Type::Comma: {
                String ret = "List:\n";
                const Array<const Node*> values = CommaToArray(this);
                for (const Node* const& value : values) ret += value->ToString(CollectionToString(padding) + '\t') + '\n';
                ReturnFromBenchmark(ret);
            }
            case Type::Array: {
                String ret = CollectionToString(padding) + "{ ";
                const Array<const Node*> params = CommaToArray(left);
                for (size_t i = 0; i < params.GetSize(); i++)
                    ret += params[i]->ToString() + (i + 1 == params.GetSize() ? " }" : ", ");
                ReturnFromBenchmark(ret);
            }
            case Type::Program: {
                String ret = CollectionToString(padding) + '[' + (value == "1" ? "!" : "") + '\n';
                const Array<const Node*> params = CommaToArray(left);
                for (size_t i = 0; i < params.GetSize(); i++)
                    ret += params[i]->ToString(CollectionToString(padding) + '\t') + (i + 1 == params.GetSize() ? ('\n'_M + padding + ']') : '\n');
                ReturnFromBenchmark(ret);
            }
            case Type::Equal:
            case Type::DynamicEqual: {
                const String tmp = right->ToString(padding);
                ReturnFromBenchmark(CollectionToString(padding) + left->ToString() + (type == Type::Equal ? " = " : " := ") + SubString(tmp, padding.GetSize(), tmp.GetSize() - padding.GetSize()));
            }
            case Type::Add: ReturnFromBenchmark(CollectionToString(padding) + '(' + left->ToString() + " + " + right->ToString() + ')');
            case Type::Sub: ReturnFromBenchmark(CollectionToString(padding) + '(' + left->ToString() + " - " + right->ToString() + ')');
            case Type::Mul: ReturnFromBenchmark(CollectionToString(padding) + '(' + left->ToString() + " * " + right->ToString() + ')');
            case Type::Div: ReturnFromBenchmark(CollectionToString(padding) + '(' + left->ToString() + " / " + right->ToString() + ')');
            case Type::Pow: ReturnFromBenchmark(CollectionToString(padding) + '(' + left->ToString() + " ^ " + right->ToString() + ')');
            case Type::Root: ReturnFromBenchmark(CollectionToString(padding) + '(' + left->ToString() + " $ " + right->ToString() + ')');
            case Type::Factorial: ReturnFromBenchmark(CollectionToString(padding) + left->ToString() + '!');
            case Type::Absolute: ReturnFromBenchmark(CollectionToString(padding) + '|' + left->ToString() + '|');
            case Type::LogicalEqual: ReturnFromBenchmark(CollectionToString(padding) + '(' + left->ToString() + " == " + right->ToString() + ')');
            case Type::LogicalNotEqual: ReturnFromBenchmark(CollectionToString(padding) + '(' + left->ToString() + " != " + right->ToString() + ')');
            case Type::LessThan: ReturnFromBenchmark(CollectionToString(padding) + '(' + left->ToString() + " < " + right->ToString() + ')');
            case Type::GreaterThan: ReturnFromBenchmark(CollectionToString(padding) + '(' + left->ToString() + " > " + right->ToString() + ')');
            case Type::LessThanEqual: ReturnFromBenchmark(CollectionToString(padding) + '(' + left->ToString() + " <= " + right->ToString() + ')');
            case Type::GreaterThanEqual: ReturnFromBenchmark(CollectionToString(padding) + '(' + left->ToString() + " >= " + right->ToString() + ')');
            case Type::Struct: {
                const Array<const Node*> fields = CommaToArray(left);
                String ret = CollectionToString(padding) + "{\n";
                for (size_t i = 0; i < fields.GetSize(); i += 2)
                    ret += fields.At(i)->ToString(CollectionToString(padding) + '\t') + " = " + fields.At(i + 1)->ToString() + '\n';
                ReturnFromBenchmark(ret + padding + '}');
            }
            case Type::Keyword: {
                if (value == "if") {
                    const Array<const Node*> params = CommaToArray(left);
                    ReturnFromBenchmark(CollectionToString(padding) + "if (" + params.At(0)->ToString() + ")\n" + params.At(1)->ToString(CollectionToString(padding) + '\t') + ((params.GetSize() == 3) ? String('\n') + padding + "else\n" + params.At(2)->ToString(CollectionToString(padding) + '\t') : ""));
                }
                else if (value == "while") {
                    const Array<const Node*> params = CommaToArray(left);
                    ReturnFromBenchmark(CollectionToString(padding) + "while (" + params.At(0)->ToString() + ")\n" + params.At(1)->ToString(CollectionToString(padding) + '\t'));
                }
                else if (value == "return") ReturnFromBenchmark(CollectionToString(padding) + "return " + left->ToString())
                else if (value == "integral") {
                    const Array<const Node*> params = CommaToArray(left);
                    ReturnFromBenchmark(CollectionToString(padding) + "integral(" + params.At(0)->ToString() + " d" + params.At(1)->ToString() + ')');
                }
                else if (value == "summation") {
                    const Array<const Node*> params = CommaToArray(left);
                    ReturnFromBenchmark(CollectionToString(padding) + "summation(" + params.At(0)->ToString() + " = " + params.At(1)->ToString() + ", " + params.At(0)->ToString() + " <= " + params.At(2)->ToString() + ", " + params.At(3)->ToString()  + ')');
                }
                else if (value == "product") {
                    const Array<const Node*> params = CommaToArray(left);
                    ReturnFromBenchmark(CollectionToString(padding) + "product(" + params.At(0)->ToString() + " = " + params.At(1)->ToString() + ", " + params.At(0)->ToString() + " <= " + params.At(2)->ToString() + ", " + params.At(3)->ToString()  + ')');
                }
                else if (value == "MakeStruct") {
                    const Array<const Node*> fields = CommaToArray(left);
                    String ret = CollectionToString(padding) + "{\n";
                    for (const Node* const& field : fields) ret += field->ToString(CollectionToString(padding) + '\t') + '\n';
                    ReturnFromBenchmark(ret + padding + '}');
                }
                else if (value == "GetField") {
                    const Array<const Node*> params = CommaToArray(left);
                    ReturnFromBenchmark(CollectionToString(padding) + "SetField(" + params.At(0)->ToString() + ", " + params.At(1)->ToString() + ", " + params.At(2)->ToString() + ')');
                }
                else if (value == "SetField") {
                    const Array<const Node*> params = CommaToArray(left);
                    ReturnFromBenchmark(CollectionToString(padding) + "SetField(" + params.At(0)->ToString() + ", " + params.At(1)->ToString() + ')');
                }
                else ReturnFromBenchmark(CollectionToString(padding) + value)
            }
            default: ReturnFromBenchmark(CollectionToString(padding) + "nan");
        }
    }
}
