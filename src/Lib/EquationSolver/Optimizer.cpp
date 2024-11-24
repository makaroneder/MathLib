#include "../Math/Factorial.hpp"
#include "Optimizer.hpp"
#include "../Host.hpp"

namespace MathLib {
    Optimizer::Optimizer(const Array<BuiltinFunction>& builtinFuncs, const Array<FunctionNode>& funcs, const Array<Variable>& vars) : builtinFunctions(CreateDefaultBuiltinFunctions()), functions(funcs), variables(CreateDefaultVariables()), runtime(false), parent(nullptr) {
        for (const BuiltinFunction& func : builtinFuncs) builtinFunctions.Add(func);
        for (const Variable& var : vars) variables.Add(var);
    }
    Optimizer::Optimizer(Optimizer* parent) : builtinFunctions(), functions(), variables(), runtime(parent->runtime), parent(parent) {}
    void Optimizer::Destroy(void) {
        for (FunctionNode& function : functions) {
            for (Variable& arg : function.arguments) delete arg.value;
            delete function.body;
        }
        for (Variable& variable : variables) delete variable.value;
        builtinFunctions = {};
        functions = {};
        variables = {};
        runtime = false;
    }
    BuiltinFunction* Optimizer::GetBuiltinFunctionInternal(const String& name) {
        for (BuiltinFunction& function : builtinFunctions)
            if (function.name == name) return &function;
        return parent ? parent->GetBuiltinFunctionInternal(name) : nullptr;
    }
    FunctionNode* Optimizer::GetFunctionInternal(const String& name) {
        for (FunctionNode& function : functions)
            if (function.name == name) return &function;
        return parent ? parent->GetFunctionInternal(name) : nullptr;
    }
    Variable* Optimizer::GetVariableInternal(const String& name) {
        for (Variable& var : variables)
            if (var.name == name) return &var;
        return parent ? parent->GetVariableInternal(name) : nullptr;
    }
    FunctionNode Optimizer::GetFunction(const String& name) const {
        for (const FunctionNode& function : functions)
            if (function.name == name) return function;
        return parent ? parent->GetFunction(name) : FunctionNode("", Array<Variable>(), nullptr, "");
    }
    Node* Optimizer::Optimize(const Node* node) {
        return OptimizeComma(node);
    }
    Node* Optimizer::OptimizeComma(const Node* node) {
        if (node->type != Node::Type::Comma) return OptimizeInternal(node);
        Node* ret = new Node(Node::Type::Comma, "");
        if (node->left != nullptr) ret->left = OptimizeComma(node->left);
        if (node->right != nullptr) ret->right = OptimizeComma(node->right);
        return ret;
    }
    Node* Optimizer::OptimizeVariable(const Node* node) {
        Variable* var = GetVariableInternal(node->value);
        return var && (var->constant || runtime) ? var->value->Recreate() : node->Recreate();
    }
    Node* Optimizer::OptimizeFunction(const Node* node) {
        BuiltinFunction* builtinFunction = GetBuiltinFunctionInternal(node->value);
        if (builtinFunction) {
            Node* f = new Node(Node::Type::Function, node->value, OptimizeComma(node->left));
            Array<const Node*> args = CommaToArray(f->left);
            for (size_t j = 0; j < args.GetSize(); j++)
                if (args.At(j)->type != Node::Type::Constant && args.At(j)->type != Node::Type::Array && args.At(j)->type != Node::Type::String) return f;
            Node* ret = builtinFunction->function(args);
            if (ret == nullptr) return f;
            delete f;
            Node* tmp = OptimizeInternal(ret);
            if (tmp == nullptr) return ret;
            delete ret;
            return tmp;
        }
        FunctionNode* function = GetFunctionInternal(node->value);
        if (function) {
            Node* comma = OptimizeComma(node->left);
            Array<const Node*> args = CommaToArray(comma);
            if (args.GetSize() == function->arguments.GetSize()) {
                Node* body = function->body->Recreate();
                for (size_t i = 0; i < args.GetSize(); i++) {
                    Node* tmp = ReplaceVariable(body, function->arguments[i].name, args[i]);
                    delete body;
                    body = tmp;
                }
                delete comma;
                Node* ret = OptimizeInternal(body);
                delete body;
                return ret;
            }
        }
        return node->Recreate();
    }
    Node* Optimizer::OptimizeProgram(const Node* node) {
        const Array<const Node*> body = CommaToArray(node->type == Node::Type::Array ? node->left : node);
        for (const Node* const instruction : body) {
            Node* tmp = OptimizeInternal(instruction);
            if (tmp->type == Node::Type::Keyword && tmp->value == "return") return tmp;
            else delete tmp;
        }
        return node->Recreate();
    }
    Node* Optimizer::OptimizeComparison(const Node* node, const Array<Node::Type>& validTypes, Node::Type defaultType) {
        Node* l = OptimizeInternal(node->left);
        Node* r = OptimizeInternal(node->right);
        Node::Type type = Node::Type::None;
        if ((l->type == Node::Type::Constant || l->type == Node::Type::Array || l->type == Node::Type::String) && (r->type == Node::Type::Constant || r->type == Node::Type::Array || r->type == Node::Type::String)) {
            if (l->type == r->type && l->value == r->value) type = Node::Type::LogicalEqual;
            else if (l->type == r->type && l->type == Node::Type::Constant) type = l->ToNumber().At(0).ToReal() < r->ToNumber().At(0).ToReal() ? Node::Type::LessThan : Node::Type::GreaterThan;
            else type = Node::Type::LogicalNotEqual;
        }
        else return new Node(defaultType, "", l, r);
        delete l;
        delete r;
        for (const Node::Type& validType : validTypes)
            if (validType == type) return new Node(Node::Type::Constant, "1");
        return new Node(Node::Type::Constant, "0");
    }
    Node* Optimizer::OptimizeInternal(const Node* node) {
        if (node->type == Node::Type::Constant || node->type == Node::Type::String) return node->Recreate();
        else if (node->type == Node::Type::Variable) return OptimizeVariable(node);
        else if (node->type == Node::Type::Function) return OptimizeFunction(node);
        else if (node->type == Node::Type::Array) return new Node(Node::Type::Array, "", OptimizeComma(node->left));
        else if (node->type == Node::Type::Program) {
            if (node->value == "1" && !runtime) return node->Recreate();
            Optimizer scope = Optimizer(this);
            Node* ret = scope.OptimizeProgram(node->left);
            scope.Destroy();
            if (ret->type == Node::Type::Keyword && ret->value == "return") {
                Node* tmp = ret->left->Recreate();
                delete ret;
                return tmp;
            }
            else return ret;
        }
        else if (node->type == Node::Type::Index) {
            Node* l = OptimizeInternal(node->left);
            Node* r = OptimizeInternal(node->right);
            if (r->type != Node::Type::Constant) return new Node(Node::Type::Index, "", l, r);
            const complex_t index = r->ToNumber().At(0).GetReal() - 1;
            if (index.GetImaginary() != 0) return new Node(Node::Type::Index, "", l, r);
            if (l->type == Node::Type::Array) {
                Node* tmp = l->ToArray().At(index.GetReal())->Recreate();
                delete l;
                delete r;
                Node* ret = OptimizeInternal(tmp);
                delete tmp;
                return ret;
            }
            else if (l->type == Node::Type::String) {
                const char tmp = l->value.At(index.GetReal());
                delete l;
                delete r;
                return new Node(Node::Type::String, tmp);
            }
            else return new Node(Node::Type::Index, "", l, r);
        }
        else if (node->type == Node::Type::Equal) {
            Node* l = OptimizeInternal(node->left);
            Node* r = OptimizeInternal(node->right);
            if (l->type == Node::Type::Variable && (r->type == Node::Type::Constant || r->type == Node::Type::Array || r->type == Node::Type::String)) {
                variables.Add(Variable(l->value, l->left->value, r->Recreate(), true));
                delete l;
                return r;
            }
            else if (l->type == Node::Type::Function && l->right != nullptr) {
                Array<Variable> args;
                Array<const Node*> nodeArgs = CommaToArray(l->left);
                for (const Node*& arg : nodeArgs) args.Add(Variable(arg->value, arg->left->value, "0", true));
                functions.Add(FunctionNode(l->value, args, r->Recreate(), l->right->value));
            }
            return new Node(Node::Type::Equal, "", l, r);
        }
        else if (node->type == Node::Type::DynamicEqual) {
            Node* value = OptimizeInternal(node->right);
            if (node->left->type != Node::Type::Variable) return new Node(Node::Type::DynamicEqual, "", node->left, value);
            if (value->type != Node::Type::Constant && value->type != Node::Type::Array && value->type != Node::Type::String) return new Node(Node::Type::DynamicEqual, "", node->left, value);
            Variable* var = GetVariableInternal(node->left->value);
            if (var) {
                if (var->constant || !runtime) return new Node(Node::Type::DynamicEqual, "", node->left, value);
                var->value = value->Recreate();
                return value;
            }
            variables.Add(Variable(node->left->value, node->left->left->value, value->Recreate(), false));
            return value;
        }
        else if (node->type == Node::Type::Absolute) {
            Node* n = OptimizeInternal(node->left);
            if (n->type == Node::Type::Constant) {
                const complex_t val = n->ToNumber().At(0);
                delete n;
                return new Node(Node::Type::Constant, ToString(Abs(val)));
            }
            else return new Node(Node::Type::Absolute, "", n);
        }
        else if (node->type == Node::Type::Factorial) {
            Node* n = OptimizeInternal(node->left);
            if (n->type == Node::Type::Constant) {
                const complex_t ret = Factorial<complex_t>(n->ToNumber().At(0), 1);
                delete n;
                return new Node(Node::Type::Constant, ret.ToString());
            }
            else return new Node(Node::Type::Factorial, "", n);
        }
        else if (node->type == Node::Type::LogicalEqual || node->type == Node::Type::LogicalNotEqual || node->type == Node::Type::LessThan || node->type == Node::Type::GreaterThan)
            return OptimizeComparison(node, MakeArrayFromSingle<Node::Type>(node->type), node->type);
        else if (node->type == Node::Type::LessThanEqual) {
            Array<Node::Type> validTypes = Array<Node::Type>(2);
            validTypes.At(0) = Node::Type::LessThan;
            validTypes.At(1) = Node::Type::LogicalEqual;
            return OptimizeComparison(node, validTypes, node->type);
        }
        else if (node->type == Node::Type::GreaterThanEqual) {
            Array<Node::Type> validTypes = Array<Node::Type>(2);
            validTypes.At(0) = Node::Type::GreaterThan;
            validTypes.At(1) = Node::Type::LogicalEqual;
            return OptimizeComparison(node, validTypes, node->type);
        }
        else if (node->type == Node::Type::Keyword) {
            if (node->value == "if") {
                Array<const Node*> arr = CommaToArray(node->left);
                Node* tmp = OptimizeInternal(arr.At(0));
                Node* body = nullptr;
                if (tmp->type == Node::Type::Constant && tmp->ToNumber().At(0).ToReal()) body = arr.At(1)->Recreate();
                else if (arr.GetSize() == 3) body = arr.At(2)->Recreate();
                delete tmp;
                if (!body) return new Node(Node::Type::Constant, "0");
                Optimizer scope = Optimizer(this);
                Node* ret = scope.OptimizeProgram(body);
                scope.Destroy();
                return ret;
            }
            else if (node->value == "while") {
                Array<const Node*> arr = CommaToArray(node->left);
                Node* req = OptimizeInternal(arr.At(0));
                if (req->type == Node::Type::Constant) {
                    Optimizer scope = Optimizer(this);
                    while (req->ToNumber().At(0).ToReal()) {
                        Node* tmp = scope.OptimizeProgram(arr.At(1));
                        delete req;
                        if (tmp->type == Node::Type::Keyword && tmp->value == "return") return tmp;
                        else delete tmp;
                        req = OptimizeInternal(arr.At(0));
                    }
                    scope.Destroy();
                }
                delete req;
            }
            else if (node->value == "integral") {
                String pow = "1";
                Node* del = OptimizeComma(node->left);
                Array<const Node*> arr = CommaToArray(del);
                const Node*& a0 = arr.At(0);
                const Node*& a1 = arr.At(1);
                if (a0->type == Node::Type::Variable) pow = "2";
                else if (a0->type == Node::Type::Constant) {
                    Node* l = a0->Recreate();
                    Node* r = a1->Recreate();
                    delete del;
                    return new Node(Node::Type::Mul, "", l, r);
                }
                else if (a0->type == Node::Type::Pow) {
                    if (a0->left->type == Node::Type::Variable && a0->left->value == a1->value && a0->right->type == Node::Type::Constant)
                        pow = ToString(a0->right->ToNumber().At(0).GetReal() + 1);
                    else {
                        delete del;
                        return node->Recreate();
                    }
                }
                else if (a0->type == Node::Type::Mul) {
                    if (a0->left->type == Node::Type::Variable && a0->left->value == a1->value) {
                        Node* tmp = new Node(Node::Type::Keyword, "integral", new Node(Node::Type::Comma, "", a0->left->Recreate(), a1->Recreate()));
                        Node* integral = OptimizeInternal(tmp);
                        delete tmp;
                        tmp = a0->right->Recreate();
                        delete del;
                        tmp = new Node(Node::Type::Mul, "", tmp, integral);
                        Node* ret = OptimizeInternal(tmp);
                        delete tmp;
                        return ret;
                    }
                    else if (a0->right->type == Node::Type::Variable && a0->right->value == a1->value) {
                        Node* tmp = new Node(Node::Type::Keyword, "integral", new Node(Node::Type::Comma, "", a0->right->Recreate(), a1->Recreate()));
                        Node* integral = OptimizeInternal(tmp);
                        delete tmp;
                        tmp = a0->left->Recreate();
                        delete del;
                        tmp = new Node(Node::Type::Mul, "", tmp, integral);
                        Node* ret = OptimizeInternal(tmp);
                        delete tmp;
                        return ret;
                    }
                    else {
                        delete del;
                        return node->Recreate();
                    }
                }
                else if (a0->type == Node::Type::Div) {
                    if (a0->left->type == Node::Type::Variable && a0->left->value == a1->value) {
                        Node* tmp = new Node(Node::Type::Keyword, "integral", new Node(Node::Type::Comma, "", a0->left->Recreate(), a1->Recreate()));
                        Node* integral = OptimizeInternal(tmp);
                        delete tmp;
                        tmp = new Node(Node::Type::Div, "", integral, a0->right->Recreate());
                        delete del;
                        Node* ret = OptimizeInternal(tmp);
                        delete tmp;
                        return ret;
                    }
                    else {
                        delete del;
                        return node->Recreate();
                    }
                }
                else {
                    delete del;
                    return node->Recreate();
                }
                Node* tmp = pow == "0" ? new Node(Node::Type::Function, "ln", new Node(Node::Type::Absolute, "", a1->Recreate())) : new Node(Node::Type::Div, "", new Node(Node::Type::Pow, "", a1->Recreate(), new Node(Node::Type::Constant, pow)), new Node(Node::Type::Constant, pow));
                delete del;
                Node* ret = OptimizeInternal(tmp);
                delete tmp;
                return ret;
            }
            else if (node->value == "summation") {
                Node* del = OptimizeComma(node->left);
                Array<const Node*> args = CommaToArray(del);
                const Node*& a0 = args.At(0);
                const Node*& a1 = args.At(1);
                const Node*& a2 = args.At(2);
                const Node*& a3 = args.At(3);
                if (a0->type != Node::Type::Variable) return new Node(Node::Type::Keyword, "summation", del);
                if (ContainsVariable(a3, a0->value)) {
                    Node* ret;
                    Node** curr = &ret;
                    if (a1->type != Node::Type::Constant || a2->type != Node::Type::Constant) return new Node(Node::Type::Keyword, "summation", del);
                    if (a2->ToNumber().At(0).GetReal() < a1->ToNumber().At(0).GetReal()) return new Node(Node::Type::Constant, "0");
                    for (num_t i = a1->ToNumber().At(0).GetReal(); i <= a2->ToNumber().At(0).GetReal(); i++) {
                        Node* tmp = new Node(Node::Type::Constant, ToString(i));
                        *curr = new Node(Node::Type::Add, "", ReplaceVariable(a3, a0->value, tmp), new Node(Node::Type::Constant, "0"));
                        delete tmp;
                        curr = &(*curr)->right;
                    }
                    delete del;
                    del = ret;
                    ret = OptimizeInternal(ret);
                    delete del;
                    return ret;
                }
                else {
                    Node* tmp = a3->Recreate();
                    Node* start = a1->Recreate();
                    Node* end = a2->Recreate();
                    delete del;
                    del = new Node(Node::Type::Mul, "", tmp, new Node(Node::Type::Add, "", new Node(Node::Type::Sub, "", end, start), new Node(Node::Type::Constant, "1")));
                    tmp = OptimizeInternal(del);
                    delete del;
                    return tmp;
                }
            }
            else if (node->value == "product") {
                Node* del = OptimizeComma(node->left);
                Array<const Node*> args = CommaToArray(del);
                const Node*& a0 = args.At(0);
                const Node*& a1 = args.At(1);
                const Node*& a2 = args.At(2);
                const Node*& a3 = args.At(3);
                if (a0->type != Node::Type::Variable) return new Node(Node::Type::Keyword, "product", del);
                if (ContainsVariable(a3, a0->value)) {
                    Node* ret;
                    Node** curr = &ret;
                    if (a1->type != Node::Type::Constant || a2->type != Node::Type::Constant) return new Node(Node::Type::Keyword, "product", del);
                    if (a2->ToNumber().At(0).GetReal() < a1->ToNumber().At(0).GetReal()) return new Node(Node::Type::Constant, "1");
                    for (num_t i = a1->ToNumber().At(0).GetReal(); i <= a2->ToNumber().At(0).GetReal(); i++) {
                        Node* tmp = new Node(Node::Type::Constant, ToString(i));
                        *curr = new Node(Node::Type::Mul, "", ReplaceVariable(a3, a0->value, tmp), new Node(Node::Type::Constant, "1"));
                        delete tmp;
                        curr = &(*curr)->right;
                    }
                    delete del;
                    del = ret;
                    ret = OptimizeInternal(ret);
                    delete del;
                    return ret;
                }
                else {
                    Node* tmp = a3->Recreate();
                    Node* start = a1->Recreate();
                    Node* end = a2->Recreate();
                    delete del;
                    del = new Node(Node::Type::Pow, "", tmp, new Node(Node::Type::Add, "", new Node(Node::Type::Sub, "", end, start), new Node(Node::Type::Constant, "1")));
                    tmp = OptimizeInternal(del);
                    delete del;
                    return tmp;
                }
            }
            return new Node(Node::Type::Keyword, node->value, node->left ? OptimizeInternal(node->left) : nullptr, node->right ? OptimizeInternal(node->right) : nullptr);
        }
        else if (node->type == Node::Type::Add) {
            Node* l = OptimizeInternal(node->left);
            Node* r = OptimizeInternal(node->right);
            if (l->type == Node::Type::Constant && r->type == Node::Type::Constant) {
                const String ret = (l->ToNumber().At(0) + r->ToNumber().At(0)).ToString();
                delete l;
                delete r;
                return new Node(Node::Type::Constant, ret);
            }
            else if (l->type == Node::Type::String && r->type == Node::Type::String) {
                const String ret = l->value + r->value;
                delete l;
                delete r;
                return new Node(Node::Type::String, ret);
            }
            else if (l->type == Node::Type::Constant && l->value == "0") {
                Node* ret = r->Recreate();
                delete l;
                delete r;
                return ret;
            }
            else if (r->type == Node::Type::Constant && r->value == "0") {
                Node* ret = l->Recreate();
                delete l;
                delete r;
                return ret;
            }
            else return new Node(Node::Type::Add, "", l, r);
        }
        else if (node->type == Node::Type::Sub) {
            Node* l = OptimizeInternal(node->left);
            Node* r = OptimizeInternal(node->right);
            if (l->type == Node::Type::Constant && r->type == Node::Type::Constant) {
                const complex_t lv = l->ToNumber().At(0);
                const complex_t rv = r->ToNumber().At(0);
                delete l;
                delete r;
                return new Node(Node::Type::Constant, (lv - rv).ToString());
            }
            else if (l->type == Node::Type::Constant && l->value == "0") {
                Node* ret = new Node(Node::Type::Mul, "", new Node(Node::Type::Constant, "-1"), r->Recreate());
                delete l;
                delete r;
                return ret;
            }
            else if (r->type == Node::Type::Constant && r->value == "0") {
                Node* ret = l->Recreate();
                delete l;
                delete r;
                return ret;
            }
            else return new Node(Node::Type::Sub, "", l, r);
        }
        else if (node->type == Node::Type::Mul) {
            Node* l = OptimizeInternal(node->left);
            Node* r = OptimizeInternal(node->right);
            if (l->type == Node::Type::Constant && r->type == Node::Type::Constant) {
                const complex_t lv = l->ToNumber().At(0);
                const complex_t rv = r->ToNumber().At(0);
                delete l;
                delete r;
                return new Node(Node::Type::Constant, (lv * rv).ToString());
            }
            else if (l->type == Node::Type::Constant && l->value == "1") {
                Node* ret = r->Recreate();
                delete l;
                delete r;
                return ret;
            }
            else if (r->type == Node::Type::Constant && r->value == "1") {
                Node* ret = l->Recreate();
                delete l;
                delete r;
                return ret;
            }
            else if ((l->type == Node::Type::Constant && l->value == "0") || (r->type == Node::Type::Constant && r->value == "0")) {
                delete l;
                delete r;
                return new Node(Node::Type::Constant, "0");
            }
            else return new Node(Node::Type::Mul, "", l, r);
        }
        else if (node->type == Node::Type::Div) {
            Node* l = OptimizeInternal(node->left);
            Node* r = OptimizeInternal(node->right);
            if (l->type == Node::Type::Constant && r->type == Node::Type::Constant) {
                const complex_t lv = l->ToNumber().At(0);
                const complex_t rv = r->ToNumber().At(0);
                delete l;
                delete r;
                return new Node(Node::Type::Constant, (lv / rv).ToString());
            }
            else if (r->type == Node::Type::Constant && r->value == "1") {
                delete r;
                return l;
            }
            else return new Node(Node::Type::Div, "", l, r);
        }
        else if (node->type == Node::Type::Pow) {
            Node* l = OptimizeInternal(node->left);
            Node* r = OptimizeInternal(node->right);
            if (l->type == Node::Type::Constant && r->type == Node::Type::Constant) {
                const complex_t lv = l->ToNumber().At(0);
                const complex_t rv = r->ToNumber().At(0);
                delete l;
                delete r;
                return new Node(Node::Type::Constant, Pow(lv, rv).ToString());
            }
            else if (r->type == Node::Type::Constant && r->value == "1") {
                delete r;
                return l;
            }
            else if (r->type == Node::Type::Constant && r->value == "0") {
                delete l;
                delete r;
                return new Node(Node::Type::Constant, "1");
            }
            else if (l->type == Node::Type::Constant && l->value == "1") {
                delete l;
                delete r;
                return new Node(Node::Type::Constant, "1");
            }
            else return new Node(Node::Type::Pow, "", l, r);
        }
        else if (node->type == Node::Type::Root) {
            Node* l = OptimizeInternal(node->left);
            Node* r = OptimizeInternal(node->right);
            if (l->type == Node::Type::Constant && r->type == Node::Type::Constant) {
                const complex_t lv = l->ToNumber().At(0);
                const complex_t rv = r->ToNumber().At(0);
                delete l;
                delete r;
                return new Node(Node::Type::Constant, Pow(rv, lv.GetInverse()).ToString());
            }
            else return new Node(Node::Type::Root, "", l, r);
        }
        else return nullptr;
    }
}