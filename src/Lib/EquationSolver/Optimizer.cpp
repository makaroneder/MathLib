#include "Optimizer.hpp"
#include "../Typedefs.hpp"
#include "../Factorial.hpp"
#include "../Functions.hpp"

Node* OptimizeInternal(Node* node, State& state);
/// @brief Optimizes given node based on the given variables and creates new variables
/// @param node Node to optimize
/// @param state Current state
/// @return Optimized node and new variables
Node* OptimizeComma(Node* node, State& state) {
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
Node* OptimizeVariable(Node* node, std::vector<Variable>& variables) {
    for (Variable& var : variables)
        if (node->value == var.name) return new Node(Node::Type::Constant, var.value);
    return node->Recreate();
}
/// @brief Optimizes given node based on the given variables and creates new variables
/// @param node Node to optimize
/// @param state Current state
/// @return Optimized node and new variables
Node* OptimizeFunction(Node* node, State& state) {
    for (BuiltinFunction& func : state.builtinFunctions) {
        if (func.name == node->value) {
            Node* f = new Node(Node::Type::Function, node->value, OptimizeComma(node->left, state));
            std::vector<Node*> args = CommaToArray(f->left);
            for (Node*& arg : args)
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
            std::vector<Node*> args = CommaToArray(comma);
            if (args.size() != func.arguments.size()) continue;
            Node* body = func.body->Recreate();
            for (size_t i = 0; i < args.size(); i++) {
                Node* tmp = ReplaceNode(body, [func, args, i](Node* node) {
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
Node* OptimizeInternal(Node* node, State& state) {
    if (node->type == Node::Type::Constant) return node->Recreate();
    else if (node->type == Node::Type::Variable) return OptimizeVariable(node, state.variables);
    else if (node->type == Node::Type::Function) return OptimizeFunction(node, state);
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
            std::vector<Node*> nodeArgs = CommaToArray(l->left);
            for (Node*& arg : nodeArgs) args.push_back(Variable(arg->value, "0"));
            state.functions.push_back(Function(l->value, args, r->Recreate()));
        }
        else if (r->type == Node::Type::Function) {
            std::vector<Variable> args;
            std::vector<Node*> nodeArgs = CommaToArray(r->left);
            for (Node*& arg : nodeArgs) args.push_back(Variable(arg->value, "0"));
            state.functions.push_back(Function(r->value, args, l->Recreate()));
        }
        return new Node(Node::Type::Equal, "", l, r);
    }
    else if (node->type == Node::Type::Absolute) {
        Node* n = OptimizeInternal(node->left, state);
        if (n->type == Node::Type::Constant) {
            std::string val = n->value;
            delete n;
            return new Node(Node::Type::Constant, std::to_string(std::abs(std::stold(val))));
        }
        else return new Node(Node::Type::Absolute, "", n);
    }
    else if (node->type == Node::Type::Factorial) {
        Node* n = OptimizeInternal(node->left, state);
        if (n->type == Node::Type::Constant) {
            std::string val = n->value;
            delete n;
            return new Node(Node::Type::Constant, std::to_string(Factorial<num_t>(std::stold(val))));
        }
        else return new Node(Node::Type::Factorial, "", n);
    }
    // TODO: Add derivatives
    else if (node->type == Node::Type::Integral) {
        std::string pow = "1";
        Node* del = OptimizeComma(node->left, state);
        std::vector<Node*> arr = CommaToArray(del);
        Node*& a0 = arr.at(0);
        Node*& a1 = arr.at(1);
        if (a0->type == Node::Type::Variable) pow = "2";
        else if (a0->type == Node::Type::Constant) {
            Node* l = a0->Recreate();
            Node* r = a1->Recreate();
            delete del;
            return new Node(Node::Type::Mul, "", l, r);
        }
        else if (a0->type == Node::Type::Pow) {
            if (a0->left->type == Node::Type::Variable && a0->left->value == a1->value)
                pow = std::to_string(std::stold(a0->right->value) + 1);
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
        Node* tmp = new Node(Node::Type::Div, "", new Node(Node::Type::Pow, "", a1->Recreate(), new Node(Node::Type::Constant, pow)), new Node(Node::Type::Constant, pow));
        delete del;
        Node* ret = Optimize(tmp, state);
        delete tmp;
        return ret;
    }
    else if (node->type == Node::Type::Add) {
        Node* l = Optimize(node->left, state);
        Node* r = Optimize(node->right, state);
        if (l->type == Node::Type::Constant && r->type == Node::Type::Constant) {
            num_t lv = std::stold(l->value);
            num_t rv = std::stold(r->value);
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
            num_t lv = std::stold(l->value);
            num_t rv = std::stold(r->value);
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
            num_t lv = std::stold(l->value);
            num_t rv = std::stold(r->value);
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
            num_t lv = std::stold(l->value);
            num_t rv = std::stold(r->value);
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
            num_t lv = std::stold(l->value);
            num_t rv = std::stold(r->value);
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
            num_t lv = std::stold(l->value);
            num_t rv = std::stold(r->value);
            delete l;
            delete r;
            return new Node(Node::Type::Constant, std::to_string(std::pow<num_t>(rv, 1 / lv)));
        }
        else return new Node(Node::Type::Root, "", l, r);
    }
    else return nullptr;
}
Node* Optimize(Node* node, State& state) {
    return OptimizeComma(node, state);
}