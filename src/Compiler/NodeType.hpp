#ifndef NodeType_H
#define NodeType_H
#include <stdint.h>

enum class NodeType : uint8_t {
    None,
    Return,
    Comma,
    Digit,
    Identifier,
    VariableDefinition,
    FunctionDefinition,
    FunctionCall,
    Addition,
    Multiplication,
};

#endif