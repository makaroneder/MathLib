#ifndef MathLib_EquationSolver_Node_H
#define MathLib_EquationSolver_Node_H
#include "../Math/Complex.hpp"

namespace MathLib {
    struct Node : Printable {
        enum class Type {
            None,
            Function, Variable, Constant, Array, String, Struct,
            Program,
            Comma,
            Equal, DynamicEqual,
            Add, Sub,
            Mul, Div,
            Pow, Root,
            Factorial,
            Absolute,
            LogicalEqual, LogicalNotEqual,
            LessThan, GreaterThan, LessThanEqual, GreaterThanEqual,
            Index,
            Keyword,
        } type;
        static constexpr const char* keywords[] = {
            "if", "while", "return", "integral", "summation", "product",
            "MakeStruct", "GetField", "SetField",
        };
        String value;
        Node* left;
        Node* right;

        Node(Type type = Type::None, const Sequence<char>& value = ""_M, Node* left = nullptr, Node* right = nullptr);
        virtual ~Node(void) override;
        [[nodiscard]] Node* Recreate(void) const;
        [[nodiscard]] Array<const Node*> ToArray(void) const;
        [[nodiscard]] Array<complex_t> ToNumber(void) const;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
    };
    [[nodiscard]] Array<const Node*> CommaToArray(const Node* node);
    [[nodiscard]] Node* ArrayToComma(const Sequence<Node*>& array);
    [[nodiscard]] Node* ReplaceVariable(const Node* node, const Sequence<char>& name, const Node* replacement);
    [[nodiscard]] bool ContainsVariable(const Node* node, const Sequence<char>& name);
}

#endif