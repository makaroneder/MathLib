#ifndef EquationSolver_Optimizer_H
#define EquationSolver_Optimizer_H
#include "BuiltinFunction.hpp"
#include "FunctionNode.hpp"

struct Optimizer : Allocatable {
    Array<BuiltinFunction> builtinFunctions;
    Array<FunctionNode> functions;
    Array<Variable> variables;
    bool runtime;

    Optimizer(const Array<BuiltinFunction>& builtinFuncs = Array<BuiltinFunction>(), const Array<FunctionNode>& funcs = Array<FunctionNode>(), const Array<Variable>& vars = Array<Variable>());
    void Destroy(void);
    FunctionNode GetFunction(const String& name) const;
    /// @brief Optimizes given node based on the given variables and creates new variables
    /// @param node Node to optimize
    /// @return Optimized node
    Node* Optimize(const Node* node);

    private:
    /// @brief Optimizes given node based on the given variables and creates new variables
    /// @param node Node to optimize
    /// @return Optimized node
    Node* OptimizeComma(const Node* node);
    /// @brief Optimizes given node based on the given variables and creates new variables
    /// @param node Node to optimize
    /// @return Optimized node
    Node* OptimizeVariable(const Node* node);
    /// @brief Optimizes given node based on the given variables and creates new variables
    /// @param node Node to optimize
    /// @return Optimized node
    Node* OptimizeFunction(const Node* node);
    /// @brief Optimizes given node based on the given variables and creates new variables
    /// @param node Node to optimize
    /// @return Optimized node
    Node* OptimizeInternal(const Node* node);
};

#endif