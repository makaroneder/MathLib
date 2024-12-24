#ifndef MathLib_EquationSolver_Optimizer_H
#define MathLib_EquationSolver_Optimizer_H
#include "BuiltinFunction.hpp"
#include "FunctionNode.hpp"

namespace MathLib {
    struct Optimizer : Allocatable {
        Array<BuiltinFunction> builtinFunctions;
        Array<FunctionNode> functions;
        Array<Variable> variables;
        bool runtime;

        Optimizer(const Array<BuiltinFunction>& builtinFuncs = Array<BuiltinFunction>(), const Array<FunctionNode>& funcs = Array<FunctionNode>(), const Array<Variable>& vars = Array<Variable>());
        Optimizer(Optimizer* parent);
        void Destroy(void);
        FunctionNode GetFunction(const String& name) const;
        /// @brief Optimizes given node based on the given variables and creates new variables
        /// @param node Node to optimize
        /// @return Optimized node
        Node* Optimize(const Node* node);

        private:
        Optimizer* parent;

        BuiltinFunction* GetBuiltinFunctionInternal(const String& name);
        FunctionNode* GetFunctionInternal(const String& name);
        Variable* GetVariableInternal(const String& name);
        Node* OptimizeComparison(const Node* node, const Array<Node::Type>& validTypes, Node::Type defaultType);
        Node* OptimizeProgram(const Node* node);
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
}

#endif