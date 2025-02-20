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

        Optimizer(const Collection<BuiltinFunction>& builtinFuncs = Array<BuiltinFunction>(), const Array<FunctionNode>& funcs = Array<FunctionNode>(), const Collection<Variable>& vars = Array<Variable>());
        Optimizer(Optimizer* parent);
        Optimizer Recreate(void) const;
        void Destroy(void);
        [[nodiscard]] FunctionNode GetFunction(const String& name) const;
        [[nodiscard]] bool SetBuiltinFunctionData(const String& name, void* data);
        /// @brief Optimizes given node based on the given variables and creates new variables
        /// @param node Node to optimize
        /// @return Optimized node
        [[nodiscard]] Node* Optimize(const Node* node);

        private:
        Optimizer* parent;

        [[nodiscard]] BuiltinFunction* GetBuiltinFunctionInternal(const String& name);
        [[nodiscard]] FunctionNode* GetFunctionInternal(const String& name);
        [[nodiscard]] Variable* GetVariableInternal(const String& name);
        [[nodiscard]] Node* OptimizeComparison(const Node* node, const Collection<Node::Type>& validTypes, Node::Type defaultType);
        [[nodiscard]] Node* OptimizeProgram(const Node* node);
        /// @brief Optimizes given node based on the given variables and creates new variables
        /// @param node Node to optimize
        /// @return Optimized node
        [[nodiscard]] Node* OptimizeComma(const Node* node);
        /// @brief Optimizes given node based on the given variables and creates new variables
        /// @param node Node to optimize
        /// @return Optimized node
        [[nodiscard]] Node* OptimizeVariable(const Node* node);
        /// @brief Optimizes given node based on the given variables and creates new variables
        /// @param node Node to optimize
        /// @return Optimized node
        [[nodiscard]] Node* OptimizeFunction(const Node* node);
        /// @brief Optimizes given node based on the given variables and creates new variables
        /// @param node Node to optimize
        /// @return Optimized node
        [[nodiscard]] Node* OptimizeInternal(const Node* node);
    };
}

#endif