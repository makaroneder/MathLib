#ifndef NodeType_H
#define NodeType_H

enum class NodeType {
    None,
    Return,
    Comma,
    Digit,
    Identifier,
    VariableDefinition,
    FunctionDefinition,
    Addition,
    Multiplication,
};

#endif