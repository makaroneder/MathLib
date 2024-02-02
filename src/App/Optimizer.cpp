#include "Optimizer.hpp"
#include <Factorial.hpp>

Node* OptimizeInternal(Node* node, std::vector<Variable>& variables, std::vector<Function>& funcs, std::vector<BuiltinFunction> builtinFuncs);
/// @brief Optimizes given node based on the given variables and creates new variables
/// @param node Node to optimize
/// @param variables Variable values to use
/// @param builtinFuncs Builtin functions
/// @return Optimized node and new variables
Node* OptimizeComma(Node* node, std::vector<Variable>& variables, std::vector<Function>& funcs, std::vector<BuiltinFunction> builtinFuncs) {
    if (node->type != Node::Type::Comma) return OptimizeInternal(node, variables, funcs, builtinFuncs);
    Node* ret = new Node(Node::Type::Comma, "");
    if (node->left != nullptr) ret->left = OptimizeComma(node->left, variables, funcs, builtinFuncs);
    if (node->right != nullptr) ret->right = OptimizeComma(node->right, variables, funcs, builtinFuncs);
    return ret;
}
/// @brief Optimizes given node based on the given variables and creates new variables
/// @param node Node to optimize
/// @param variables Variable values to use
/// @return Optimized node and new variables
Node* OptimizeVariable(Node* node, std::vector<Variable>& variables) {
    for (Variable& var : variables)
        if (node->value == var.name) return new Node(Node::Type::Constant, var.value);
    return node->Recreate();
}
/// @brief Optimizes given node based on the given variables and creates new variables
/// @param node Node to optimize
/// @param variables Variable values to use
/// @param builtinFuncs Builtin functions
/// @return Optimized node and new variables
Node* OptimizeFunction(Node* node, std::vector<Variable>& variables, std::vector<Function>& funcs, std::vector<BuiltinFunction> builtinFuncs) {
    for (Function& func : funcs) {
        if (node->value == func.name) {
            Node* comma = OptimizeComma(node->left, variables, funcs, builtinFuncs);
            std::vector<Node*> args = CommaToArray(comma);
            std::vector<Variable> vars = variables;
            if (args.size() != func.arguments.size()) continue;
            for (size_t i = 0; i < args.size(); i++) {
                size_t index = SIZE_MAX;
                for (size_t j = 0; j < vars.size() && index == SIZE_MAX; j++)
                    if (vars[j].name == func.arguments[i].name) index = j;
                if (args[i]->type != Node::Type::Constant) {
                    delete comma;
                    return node->Recreate();
                }
                if (index == SIZE_MAX) vars.push_back(Variable(func.arguments[i].name, args[i]->value));
                else vars[index].value = args[i]->value;
            }
            delete comma;
            return OptimizeInternal(func.body, vars, funcs, builtinFuncs);
        }
    }
    return node->Recreate();
}

#define OptimizeOperator(nodeType, operator)                                                                            \
if (node->type == Node::Type::nodeType) {                                                                               \
    Node* l = OptimizeInternal(node->left, variables, funcs, builtinFuncs);                                             \
    Node* r = OptimizeInternal(node->right, variables, funcs, builtinFuncs);                                            \
    if (l->type == Node::Type::Constant && r->type == Node::Type::Constant) {                                           \
        num_t lv = std::stold(l->value);                                                                                \
        num_t rv = std::stold(r->value);                                                                                \
        delete l;                                                                                                       \
        delete r;                                                                                                       \
        return new Node(Node::Type::Constant, std::to_string(operator(lv, rv)));                                        \
    }                                                                                                                   \
    else return new Node(Node::Type::nodeType, "", l, r);                                                               \
}
/// @brief Optimizes given node based on the given variables and creates new variables
/// @param node Node to optimize
/// @param variables Variable values to use
/// @param builtinFuncs Builtin functions
/// @return Optimized node and new variables
Node* OptimizeInternal(Node* node, std::vector<Variable>& variables, std::vector<Function>& funcs, std::vector<BuiltinFunction> builtinFuncs) {
    if (node->type == Node::Type::Constant) return node->Recreate();
    else if (node->type == Node::Type::Function) {
        for (BuiltinFunction& func : builtinFuncs) {
            if (func.name == node->value) {
                Node* f = new Node(Node::Type::Function, node->value, OptimizeComma(node->left, variables, funcs, builtinFuncs));
                std::vector<Node*> args = CommaToArray(f->left);
                for (Node*& arg : args)
                    if (arg->type != Node::Type::Constant) return f;
                Node* ret = func.function(args);
                if (ret == nullptr) return f;
                delete f;
                return ret;
            }
        }
        return OptimizeFunction(node, variables, funcs, builtinFuncs);
    }
    else if (node->type == Node::Type::Variable) return OptimizeVariable(node, variables);
    else if (node->type == Node::Type::Equal) {
        Node* l = OptimizeInternal(node->left, variables, funcs, builtinFuncs);
        Node* r = OptimizeInternal(node->right, variables, funcs, builtinFuncs);
        if (l->type == Node::Type::Variable && r->type == Node::Type::Constant) {
            variables.push_back(Variable(l->value, r->value));
            return r;
        }
        else if (r->type == Node::Type::Variable && l->type == Node::Type::Constant) {
            variables.push_back(Variable(r->value, l->value));
            return l;
        }
        else if (l->type == Node::Type::Function) {
            std::vector<Variable> args;
            std::vector<Node*> nodeArgs = CommaToArray(l->left);
            for (Node*& arg : nodeArgs) args.push_back(Variable(arg->value, "0"));
            funcs.push_back(Function(l->value, args, r->Recreate()));
        }
        else if (r->type == Node::Type::Function) {
            std::vector<Variable> args;
            std::vector<Node*> nodeArgs = CommaToArray(r->left);
            for (Node*& arg : nodeArgs) args.push_back(Variable(arg->value, "0"));
            funcs.push_back(Function(r->value, args, l->Recreate()));
        }
        return new Node(Node::Type::Equal, "", l, r);
    }
    else if (node->type == Node::Type::Absolute) {
        Node* n = OptimizeInternal(node->left, variables, funcs, builtinFuncs);
        if (n->type == Node::Type::Constant) {
            std::string val = n->value;
            delete n;
            return new Node(Node::Type::Constant, std::to_string(std::abs(std::stold(val))));
        }
        else return new Node(Node::Type::Absolute, "", n);
    }
    else if (node->type == Node::Type::Factorial) {
        Node* n = OptimizeInternal(node->left, variables, funcs, builtinFuncs);
        if (n->type == Node::Type::Constant) {
            std::string val = n->value;
            delete n;
            return new Node(Node::Type::Constant, std::to_string(Factorial<num_t>(std::stold(val))));
        }
        else return new Node(Node::Type::Factorial, "", n);
    }
    else OptimizeOperator(Add, [](num_t x, num_t y) { return x + y; })
    else OptimizeOperator(Sub, [](num_t x, num_t y) { return x - y; })
    else OptimizeOperator(Mul, [](num_t x, num_t y) { return x * y; })
    else OptimizeOperator(Div, [](num_t x, num_t y) { return x / y; })
    else OptimizeOperator(Pow, [](num_t x, num_t y) { return std::pow<num_t>(x, y); })
    else OptimizeOperator(Root, [](num_t x, num_t y) { return std::pow<num_t>(y, 1 / x); })
    else return nullptr;
}
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
Node* Optimize(Node* node, std::vector<Variable>& variables, std::vector<Function>& funcs, std::vector<BuiltinFunction> builtinFuncs) {
    return OptimizeComma(node, variables, funcs, builtinFuncs);
}