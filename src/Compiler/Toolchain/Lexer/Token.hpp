#ifndef Token_H
#define Token_H
#include <Interfaces/Printable.hpp>

struct Token : MathLib::Printable {
    Token(void);
    Token(size_t type, const MathLib::Sequence<char>& value);
    [[nodiscard]] bool CheckType(size_t t) const;
    [[nodiscard]] MathLib::String GetValue(void) const;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    [[nodiscard]] bool operator==(const Token& other) const;
    [[nodiscard]] bool operator!=(const Token& other) const;
    
    private:
    MathLib::String value;
    size_t type;
};

#endif