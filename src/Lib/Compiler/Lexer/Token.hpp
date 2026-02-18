#ifndef MathLib_Compiler_Lexer_Token_H
#define MathLib_Compiler_Lexer_Token_H
#include "../../Interfaces/Printable.hpp"

namespace MathLib {
    struct Token : Printable {
        Token(void);
        Token(size_t type, const Sequence<char>& value);
        [[nodiscard]] bool CheckType(size_t t) const;
        [[nodiscard]] String GetValue(void) const;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
        [[nodiscard]] bool operator==(const Token& other) const;
        [[nodiscard]] bool operator!=(const Token& other) const;
        
        private:
        String value;
        size_t type;
    };
}

#endif