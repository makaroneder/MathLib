#ifndef Tokenizer_H
#define Tokenizer_H
#include "Node.hpp"

/// @brief Checks if character is white space
/// @param chr Character to check
/// @return Is character white space
bool IsWhiteSpace(char chr);
/// @brief Skips whitespace characters
/// @param str String to skip
/// @param i Current position in the string
void SkipWhiteSpace(const String str, size_t& i);
/// @brief Tokenizes string into nodes
/// @param str String to tokenize
/// @return Tokenized string
Node* Tokenize(const String str);

#endif