#include "Optimizer.hpp"
#include "../Host.hpp"
#include "../Factorial.hpp"

Node* OptimizeInternal(const Node* node, EquationSolverState& state);
/// @brief Optimizes given node based on the given variables and creates new variables
/// @param node Node to optimize
/// @param state Current state
/// @return Optimized node and new variables
Node* OptimizeComma(const Node* node, EquationSolverState& state) {
    if (node->type != Node::Type::Comma) return OptimizeInternal(node, state);
    Node* ret = new Node(Node::Type::Comma, "");
    if (node->left != nullptr) ret->left = OptimizeComma(node->left, state);
    if (node->right != nullptr) ret->right = OptimizeComma(node->right, state);
    return ret;
}
/// @brief Optimizes given node based on the given variables and creates new variables
/// @param node Node to optimize
/// @param state Current state
/// @return Optimized node and new variables
Node* OptimizeVariable(const Node* node, const EquationSolverState& state) {
    for (const Variable& variable : state.variables)
        if ((variable.constant || state.runtime) && node->value == variable.name) return variable.value->Recreate();
    return node->Recreate();
}
/// @brief Optimizes given node based on the given variables and creates new variables
/// @param node Node to optimize
/// @param state Current state
/// @return Optimized node and new variables
Node* OptimizeFunction(const Node* node, EquationSolverState& state) {
    for (const BuiltinFunction& function : state.builtinFunctions) {
        if (function.name == node->value) {
            Node* f = new Node(Node::Type::Function, node->value, OptimizeComma(node->left, state));
            Array<const Node*> args = CommaToArray(f->left);
            for (size_t j = 0; j < args.GetSize(); j++)
                if (args.At(j)->type != Node::Type::Constant && args.At(j)->type != Node::Type::Array && args.At(j)->type != Node::Type::String) return f;
            Node* ret = function.function(args);
            if (ret == nullptr) return f;
            delete f;
            Node* tmp = OptimizeInternal(ret, state);
            if (tmp == nullptr) return ret;
            delete ret;
            return tmp;
        }
    }
    for (const FunctionNode& function : state.functions) {
        if (node->value == function.name) {
            Node* comma = OptimizeComma(node->left, state);
            Array<const Node*> args = CommaToArray(comma);
            if (args.GetSize() != function.arguments.GetSize()) continue;
            Node* body = function.body->Recreate();
            for (size_t i = 0; i < args.GetSize(); i++) {
                Node* tmp = ReplaceVariable(body, function.arguments[i].name, args[i]);
                delete body;
                body = tmp;
            }
            delete comma;
            Node* ret = OptimizeInternal(body, state);
            delete body;
            return ret;
        }
    }
    return node->Recreate();
}
/// @brief Optimizes given node based on the given variables and creates new variables
/// @param node Node to optimize
/// @param state Current state
/// @return Optimized node and new variables
Node* OptimizeInternal(const Node* node, EquationSolverState& state) {
    if (node->type == Node::Type::Constant || node->type == Node::Type::String) return node->Recreate();
    else if (node->type == Node::Type::Variable) return OptimizeVariable(node, state);
    else if (node->type == Node::Type::Function) return OptimizeFunction(node, state);
    else if (node->type == Node::Type::Array) return new Node(Node::Type::Array, "", OptimizeComma(node->left, state));
    else if (node->type == Node::Type::Index) {
        Node* l = OptimizeInternal(node->left, state);
        Node* r = OptimizeInternal(node->right, state);
        if (r->type != Node::Type::Constant) return new Node(Node::Type::Index, "", l, r);
        const complex_t index = r->ToNumber().At(0).GetReal() - 1;
        if (index.GetImaginary() != 0) return new Node(Node::Type::Index, "", l, r);
        if (l->type == Node::Type::Array) {
            const complex_t tmp = l->ToNumber().At(index.GetReal());
            delete l;
            delete r;
            return new Node(Node::Type::Constant, tmp.ToString());
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
        Node* l = OptimizeInternal(node->left, state);
        Node* r = OptimizeInternal(node->right, state);
        if (l->type == Node::Type::Variable && (r->type == Node::Type::Constant || r->type == Node::Type::Array || r->type == Node::Type::String)) {
            state.variables.Add(Variable(l->value, r->Recreate(), true));
            delete l;
            return r;
        }
        else if (l->type == Node::Type::Function && l->right != nullptr) {
            Array<Variable> args;
            Array<const Node*> nodeArgs = CommaToArray(l->left);
            Array<const Node*> sets = CommaToArray(l->right);
            for (const Node*& arg : nodeArgs) args.Add(Variable(arg->value, "0", true));
            state.functions.Add(FunctionNode(l->value, args, r->Recreate(), sets.At(0)->value, sets.At(1)->value));
        }
        return new Node(Node::Type::Equal, "", l, r);
    }
    else if (node->type == Node::Type::DynamicEqual) {
        Node* value = OptimizeInternal(node->right, state);
        if (node->left->type != Node::Type::Variable) return new Node(Node::Type::DynamicEqual, "", node->left, value);
        if (value->type != Node::Type::Constant && value->type != Node::Type::Array && value->type != Node::Type::String) return new Node(Node::Type::DynamicEqual, "", node->left, value);
        for (Variable& variable : state.variables) {
            if (variable.name == node->left->value) {
                if (variable.constant || !state.runtime) return new Node(Node::Type::DynamicEqual, "", node->left, value);
                variable.value = value->Recreate();
                return value;
            }
        }
        state.variables.Add(Variable(node->left->value, value->Recreate(), false));
        return value;
    }
    else if (node->type == Node::Type::Absolute) {
        Node* n = OptimizeInternal(node->left, state);
        if (n->type == Node::Type::Constant) {
            const complex_t val = n->ToNumber().At(0);
            delete n;
            return new Node(Node::Type::Constant, ToString(Abs(val)));
        }
        else return new Node(Node::Type::Absolute, "", n);
    }
    else if (node->type == Node::Type::Factorial) {
        Node* n = OptimizeInternal(node->left, state);
        if (n->type == Node::Type::Constant) {
            const complex_t ret = Factorial<complex_t>(n->ToNumber().At(0), 1);
            delete n;
            return new Node(Node::Type::Constant, ret.ToString());
        }
        else return new Node(Node::Type::Factorial, "", n);
    }
    else if (node->type == Node::Type::Integral) {
        String pow = "1";
        Node* del = OptimizeComma(node->left, state);
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
                Node* tmp = new Node(Node::Type::Integral, "", new Node(Node::Type::Comma, "", a0->left->Recreate(), a1->Recreate()));
                Node* integral = OptimizeInternal(tmp, state);
                delete tmp;
                tmp = a0->right->Recreate();
                delete del;
                tmp = new Node(Node::Type::Mul, "", tmp, integral);
                Node* ret = OptimizeInternal(tmp, state);
                delete tmp;
                return ret;
            }
            else if (a0->right->type == Node::Type::Variable && a0->right->value == a1->value) {
                Node* tmp = new Node(Node::Type::Integral, "", new Node(Node::Type::Comma, "", a0->right->Recreate(), a1->Recreate()));
                Node* integral = OptimizeInternal(tmp, state);
                delete tmp;
                tmp = a0->left->Recreate();
                delete del;
                tmp = new Node(Node::Type::Mul, "", tmp, integral);
                Node* ret = OptimizeInternal(tmp, state);
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
                Node* tmp = new Node(Node::Type::Integral, "", new Node(Node::Type::Comma, "", a0->left->Recreate(), a1->Recreate()));
                Node* integral = OptimizeInternal(tmp, state);
                delete tmp;
                tmp = new Node(Node::Type::Div, "", integral, a0->right->Recreate());
                delete del;
                Node* ret = OptimizeInternal(tmp, state);
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
        Node* ret = OptimizeInternal(tmp, state);
        delete tmp;
        return ret;
    }
    else if (node->type == Node::Type::Summation) {
        Node* del = OptimizeComma(node->left, state);
        Array<const Node*> args = CommaToArray(del);
        const Node*& a0 = args.At(0);
        const Node*& a1 = args.At(1);
        const Node*& a2 = args.At(2);
        const Node*& a3 = args.At(3);
        if (a0->type != Node::Type::Variable) return new Node(Node::Type::Summation, "", del);
        if (ContainsVariable(a3, a0->value)) {
            Node* ret;
            Node** curr = &ret;
            if (a1->type != Node::Type::Constant || a2->type != Node::Type::Constant) return new Node(Node::Type::Summation, "", del);
            if (a2->ToNumber().At(0).GetReal() < a1->ToNumber().At(0).GetReal()) return new Node(Node::Type::Constant, "0");
            for (num_t i = a1->ToNumber().At(0).GetReal(); i <= a2->ToNumber().At(0).GetReal(); i++) {
                Node* tmp = new Node(Node::Type::Constant, ToString(i));
                *curr = new Node(Node::Type::Add, "", ReplaceVariable(a3, a0->value, tmp), new Node(Node::Type::Constant, "0"));
                delete tmp;
                curr = &(*curr)->right;
            }
            delete del;
            del = ret;
            ret = OptimizeInternal(ret, state);
            delete del;
            return ret;
        }
        else {
            Node* tmp = a3->Recreate();
            Node* start = a1->Recreate();
            Node* end = a2->Recreate();
            delete del;
            del = new Node(Node::Type::Mul, "", tmp, new Node(Node::Type::Add, "", new Node(Node::Type::Sub, "", end, start), new Node(Node::Type::Constant, "1")));
            tmp = OptimizeInternal(del, state);
            delete del;
            return tmp;
        }
    }
    else if (node->type == Node::Type::Product) {
        Node* del = OptimizeComma(node->left, state);
        Array<const Node*> args = CommaToArray(del);
        const Node*& a0 = args.At(0);
        const Node*& a1 = args.At(1);
        const Node*& a2 = args.At(2);
        const Node*& a3 = args.At(3);
        if (a0->type != Node::Type::Variable) return new Node(Node::Type::Product, "", del);
        if (ContainsVariable(a3, a0->value)) {
            Node* ret;
            Node** curr = &ret;
            if (a1->type != Node::Type::Constant || a2->type != Node::Type::Constant) return new Node(Node::Type::Product, "", del);
            if (a2->ToNumber().At(0).GetReal() < a1->ToNumber().At(0).GetReal()) return new Node(Node::Type::Constant, "1");
            for (num_t i = a1->ToNumber().At(0).GetReal(); i <= a2->ToNumber().At(0).GetReal(); i++) {
                Node* tmp = new Node(Node::Type::Constant, ToString(i));
                *curr = new Node(Node::Type::Mul, "", ReplaceVariable(a3, a0->value, tmp), new Node(Node::Type::Constant, "1"));
                delete tmp;
                curr = &(*curr)->right;
            }
            delete del;
            del = ret;
            ret = OptimizeInternal(ret, state);
            delete del;
            return ret;
        }
        else {
            Node* tmp = a3->Recreate();
            Node* start = a1->Recreate();
            Node* end = a2->Recreate();
            delete del;
            del = new Node(Node::Type::Pow, "", tmp, new Node(Node::Type::Add, "", new Node(Node::Type::Sub, "", end, start), new Node(Node::Type::Constant, "1")));
            tmp = OptimizeInternal(del, state);
            delete del;
            return tmp;
        }
    }
    else if (node->type == Node::Type::Add) {
        Node* l = OptimizeInternal(node->left, state);
        Node* r = OptimizeInternal(node->right, state);
        if (l->type == Node::Type::Constant && r->type == Node::Type::Constant) {
            const complex_t lv = l->ToNumber().At(0);
            const complex_t rv = r->ToNumber().At(0);
            delete l;
            delete r;
            return new Node(Node::Type::Constant, (lv + rv).ToString());
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
        Node* l = OptimizeInternal(node->left, state);
        Node* r = OptimizeInternal(node->right, state);
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
        Node* l = OptimizeInternal(node->left, state);
        Node* r = OptimizeInternal(node->right, state);
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
        Node* l = OptimizeInternal(node->left, state);
        Node* r = OptimizeInternal(node->right, state);
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
        Node* l = OptimizeInternal(node->left, state);
        Node* r = OptimizeInternal(node->right, state);
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
        Node* l = OptimizeInternal(node->left, state);
        Node* r = OptimizeInternal(node->right, state);
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
Node* Optimize(const Node* node, EquationSolverState& state) {
    return OptimizeComma(node, state);
}