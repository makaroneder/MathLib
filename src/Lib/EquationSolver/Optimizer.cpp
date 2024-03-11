#include "Optimizer.hpp"
#include "../Typedefs.hpp"
#include "../Factorial.hpp"
#include "../Functions.hpp"

// TODO: Add derivatives and support for complex numbers
Node* OptimizeInternal(const Node* node, State& state);
/// @brief Optimizes given node based on the given variables and creates new variables
/// @param node Node to optimize
/// @param state Current state
/// @return Optimized node and new variables
Node* OptimizeComma(const Node* node, State& state) {
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
Node* OptimizeVariable(const Node* node, std::vector<Variable>& variables) {
    for (Variable& var : variables)
        if (node->value == var.name) return new Node(Node::Type::Constant, var.value);
    return node->Recreate();
}
/// @brief Optimizes given node based on the given variables and creates new variables
/// @param node Node to optimize
/// @param state Current state
/// @return Optimized node and new variables
Node* OptimizeFunction(const Node* node, State& state) {
    for (BuiltinFunction& func : state.builtinFunctions) {
        if (func.name == node->value) {
            Node* f = new Node(Node::Type::Function, node->value, OptimizeComma(node->left, state));
            std::vector<const Node*> args = CommaToArray(f->left);
            for (const Node*& arg : args)
                if (arg->type != Node::Type::Constant) return f;
            Node* ret = func.function(args);
            if (ret == nullptr) return f;
            delete f;
            return ret;
        }
    }
    for (Function& func : state.functions) {
        if (node->value == func.name) {
            Node* comma = OptimizeComma(node->left, state);
            std::vector<const Node*> args = CommaToArray(comma);
            if (args.size() != func.arguments.size()) continue;
            Node* body = func.body->Recreate();
            for (size_t i = 0; i < args.size(); i++) {
                Node* tmp = ReplaceNode(body, [func, args, i](const Node* node) {
                    if (node->type == Node::Type::Variable && node->value == func.arguments[i].name)
                        return args[i]->Recreate();
                    return node->Recreate();
                });
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
Node* OptimizeInternal(const Node* node, State& state) {
    if (node->type == Node::Type::Constant) return node->Recreate();
    else if (node->type == Node::Type::Variable) return OptimizeVariable(node, state.variables);
    else if (node->type == Node::Type::Function) return OptimizeFunction(node, state);
    else if (node->type == Node::Type::Array) return new Node(Node::Type::Array, "", OptimizeComma(node->left, state));
    else if (node->type == Node::Type::Index) {
        Node* l = OptimizeInternal(node->left, state);
        Node* r = OptimizeInternal(node->right, state);
        if (l->type == Node::Type::Array && r->type == Node::Type::Constant) {
            std::string tmp = std::to_string(l->ToNumber().at(r->ToNumber().at(0) - 1));
            delete l;
            delete r;
            return new Node(Node::Type::Constant, tmp);
        }
        else return new Node(Node::Type::Index, "", l, r);
    }
    else if (node->type == Node::Type::Equal) {
        Node* l = OptimizeInternal(node->left, state);
        Node* r = OptimizeInternal(node->right, state);
        if (l->type == Node::Type::Variable && r->type == Node::Type::Constant) {
            state.variables.push_back(Variable(l->value, r->value));
            return r;
        }
        else if (r->type == Node::Type::Variable && l->type == Node::Type::Constant) {
            state.variables.push_back(Variable(r->value, l->value));
            return l;
        }
        else if (l->type == Node::Type::Function) {
            std::vector<Variable> args;
            std::vector<const Node*> nodeArgs = CommaToArray(l->left);
            for (const Node*& arg : nodeArgs) args.push_back(Variable(arg->value, "0"));
            state.functions.push_back(Function(l->value, args, r->Recreate()));
        }
        else if (r->type == Node::Type::Function) {
            std::vector<Variable> args;
            std::vector<const Node*> nodeArgs = CommaToArray(r->left);
            for (const Node*& arg : nodeArgs) args.push_back(Variable(arg->value, "0"));
            state.functions.push_back(Function(r->value, args, l->Recreate()));
        }
        return new Node(Node::Type::Equal, "", l, r);
    }
    else if (node->type == Node::Type::Absolute) {
        Node* n = OptimizeInternal(node->left, state);
        if (n->type == Node::Type::Constant) {
            const num_t val = n->ToNumber().at(0);
            delete n;
            return new Node(Node::Type::Constant, std::to_string(std::abs(val)));
        }
        else return new Node(Node::Type::Absolute, "", n);
    }
    else if (node->type == Node::Type::Factorial) {
        Node* n = OptimizeInternal(node->left, state);
        if (n->type == Node::Type::Constant) {
            const num_t val = n->ToNumber().at(0);
            delete n;
            return new Node(Node::Type::Constant, std::to_string(Factorial<num_t>(val)));
        }
        else return new Node(Node::Type::Factorial, "", n);
    }
    else if (node->type == Node::Type::Integral) {
        std::string pow = "1";
        Node* del = OptimizeComma(node->left, state);
        std::vector<const Node*> arr = CommaToArray(del);
        const Node*& a0 = arr.at(0);
        const Node*& a1 = arr.at(1);
        if (a0->type == Node::Type::Variable) pow = "2";
        else if (a0->type == Node::Type::Constant) {
            Node* l = a0->Recreate();
            Node* r = a1->Recreate();
            delete del;
            return new Node(Node::Type::Mul, "", l, r);
        }
        else if (a0->type == Node::Type::Pow) {
            if (a0->left->type == Node::Type::Variable && a0->left->value == a1->value)
                pow = std::to_string(a0->right->ToNumber().at(0) + 1);
            else {
                delete del;
                return node->Recreate();
            }
        }
        else if (a0->type == Node::Type::Mul) {
            if (a0->left->type == Node::Type::Variable && a0->left->value == a1->value) {
                Node* tmp = new Node(Node::Type::Integral, "", new Node(Node::Type::Comma, "", a0->left->Recreate(), a1->Recreate()));
                Node* integral = Optimize(tmp, state);
                delete tmp;
                tmp = a0->right->Recreate();
                delete del;
                tmp = new Node(Node::Type::Mul, "", tmp, integral);
                Node* ret = Optimize(tmp, state);
                delete tmp;
                return ret;
            }
            else if (a0->right->type == Node::Type::Variable && a0->right->value == a1->value) {
                Node* tmp = new Node(Node::Type::Integral, "", new Node(Node::Type::Comma, "", a0->right->Recreate(), a1->Recreate()));
                Node* integral = Optimize(tmp, state);
                delete tmp;
                tmp = a0->left->Recreate();
                delete del;
                tmp = new Node(Node::Type::Mul, "", tmp, integral);
                Node* ret = Optimize(tmp, state);
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
                Node* integral = Optimize(tmp, state);
                delete tmp;
                tmp = a0->right->Recreate();
                delete del;
                tmp = new Node(Node::Type::Div, "", integral, tmp);
                Node* ret = Optimize(tmp, state);
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
        Node* ret = Optimize(tmp, state);
        delete tmp;
        return ret;
    }
    else if (node->type == Node::Type::Summation) {
        Node* del = OptimizeComma(node->left, state);
        std::vector<const Node*> args = CommaToArray(del);
        const Node*& a0 = args.at(0);
        const Node*& a1 = args.at(1);
        const Node*& a2 = args.at(2);
        const Node*& a3 = args.at(3);
        if (a0->type != Node::Type::Variable) return new Node(Node::Type::Summation, "", del);
        if (ContainsNode(a3, [a0](const Node* node) {
            return node->type == Node::Type::Variable && node->value == a0->value;
        })) {
            Node* ret;
            Node** curr = &ret;
            if (a1->type != Node::Type::Constant || a2->type != Node::Type::Constant) return new Node(Node::Type::Summation, "", del);
            for (num_t i = a1->ToNumber().at(0); i <= a2->ToNumber().at(0); i++) {
                *curr = new Node(Node::Type::Add, "", ReplaceNode(a3, [i, a0](const Node* node) {
                    if (node->type == Node::Type::Variable && node->value == a0->value)
                        return new Node(Node::Type::Constant, std::to_string(i));
                    return node->Recreate();
                }), new Node(Node::Type::Constant, "0"));
                curr = &(*curr)->right;
            }
            delete del;
            del = ret;
            ret = Optimize(ret, state);
            delete del;
            return ret;
        }
        else {
            Node* tmp = a3->Recreate();
            Node* start = a1->Recreate();
            Node* end = a2->Recreate();
            delete del;
            del = new Node(Node::Type::Mul, "", tmp, new Node(Node::Type::Add, "", new Node(Node::Type::Sub, "", end, start), new Node(Node::Type::Constant, "1")));
            tmp = Optimize(del, state);
            delete del;
            return tmp;
        }
    }
    else if (node->type == Node::Type::Product) {
        Node* del = OptimizeComma(node->left, state);
        std::vector<const Node*> args = CommaToArray(del);
        const Node*& a0 = args.at(0);
        const Node*& a1 = args.at(1);
        const Node*& a2 = args.at(2);
        const Node*& a3 = args.at(3);
        if (a0->type != Node::Type::Variable) return new Node(Node::Type::Product, "", del);
        if (ContainsNode(a3, [a0](const Node* node) {
            return node->type == Node::Type::Variable && node->value == a0->value;
        })) {
            Node* ret;
            Node** curr = &ret;
            if (a1->type != Node::Type::Constant || a2->type != Node::Type::Constant) return new Node(Node::Type::Summation, "", del);
            for (num_t i = a1->ToNumber().at(0); i <= a2->ToNumber().at(0); i++) {
                *curr = new Node(Node::Type::Mul, "", ReplaceNode(a3, [i, a0](const Node* node) {
                    if (node->type == Node::Type::Variable && node->value == a0->value)
                        return new Node(Node::Type::Constant, std::to_string(i));
                    return node->Recreate();
                }), new Node(Node::Type::Constant, "1"));
                curr = &(*curr)->right;
            }
            delete del;
            del = ret;
            ret = Optimize(ret, state);
            delete del;
            return ret;
        }
        else {
            Node* tmp = a3->Recreate();
            Node* start = a1->Recreate();
            Node* end = a2->Recreate();
            delete del;
            del = new Node(Node::Type::Pow, "", tmp, new Node(Node::Type::Add, "", new Node(Node::Type::Sub, "", end, start), new Node(Node::Type::Constant, "1")));
            tmp = Optimize(del, state);
            delete del;
            return tmp;
        }
    }
    else if (node->type == Node::Type::Add) {
        Node* l = Optimize(node->left, state);
        Node* r = Optimize(node->right, state);
        if (l->type == Node::Type::Constant && r->type == Node::Type::Constant) {
            const num_t lv = l->ToNumber().at(0);
            const num_t rv = r->ToNumber().at(0);
            delete l;
            delete r;
            return new Node(Node::Type::Constant, std::to_string(lv + rv));
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
        Node* l = Optimize(node->left, state);
        Node* r = Optimize(node->right, state);
        if (l->type == Node::Type::Constant && r->type == Node::Type::Constant) {
            const num_t lv = l->ToNumber().at(0);
            const num_t rv = r->ToNumber().at(0);
            delete l;
            delete r;
            return new Node(Node::Type::Constant, std::to_string(lv - rv));
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
        Node* l = Optimize(node->left, state);
        Node* r = Optimize(node->right, state);
        if (l->type == Node::Type::Constant && r->type == Node::Type::Constant) {
            const num_t lv = l->ToNumber().at(0);
            const num_t rv = r->ToNumber().at(0);
            delete l;
            delete r;
            return new Node(Node::Type::Constant, std::to_string(lv * rv));
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
        Node* l = Optimize(node->left, state);
        Node* r = Optimize(node->right, state);
        if (l->type == Node::Type::Constant && r->type == Node::Type::Constant) {
            const num_t lv = l->ToNumber().at(0);
            const num_t rv = r->ToNumber().at(0);
            delete l;
            delete r;
            return new Node(Node::Type::Constant, std::to_string(lv / rv));
        }
        else if (r->type == Node::Type::Constant && r->value == "1") {
            Node* ret = l->Recreate();
            delete l;
            delete r;
            return ret;
        }
        else return new Node(Node::Type::Div, "", l, r);
    }
    else if (node->type == Node::Type::Pow) {
        Node* l = Optimize(node->left, state);
        Node* r = Optimize(node->right, state);
        if (l->type == Node::Type::Constant && r->type == Node::Type::Constant) {
            const num_t lv = l->ToNumber().at(0);
            const num_t rv = r->ToNumber().at(0);
            delete l;
            delete r;
            return new Node(Node::Type::Constant, std::to_string(std::pow<num_t>(lv, rv)));
        }
        else if (r->type == Node::Type::Constant && r->value == "1") {
            Node* ret = l->Recreate();
            delete l;
            delete r;
            return ret;
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
        Node* l = Optimize(node->left, state);
        Node* r = Optimize(node->right, state);
        if (l->type == Node::Type::Constant && r->type == Node::Type::Constant) {
            const num_t lv = l->ToNumber().at(0);
            const num_t rv = r->ToNumber().at(0);
            delete l;
            delete r;
            return new Node(Node::Type::Constant, std::to_string(std::pow<num_t>(rv, 1 / lv)));
        }
        else return new Node(Node::Type::Root, "", l, r);
    }
    else return nullptr;
}
Node* Optimize(const Node* node, State& state) {
    return OptimizeComma(node, state);
}