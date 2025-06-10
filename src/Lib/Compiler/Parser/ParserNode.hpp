#ifndef MathLib_Compiler_Parser_Node_H
#define MathLib_Compiler_Parser_Node_H
#include "../../Interfaces/Printable.hpp"

namespace MathLib {
    struct ParserNode : Collection<ParserNode>, Printable {
        ParserNode(void);
        ParserNode(size_t type, const Sequence<char>& value, const Sequence<ParserNode>& children);
        [[nodiscard]] size_t GetType(void) const;
        [[nodiscard]] String GetData(void) const;
        [[nodiscard]] virtual ParserNode At(size_t index) const override;
        [[nodiscard]] virtual ParserNode& At(size_t index) override;
        [[nodiscard]] virtual bool Add(const ParserNode& val) override;
        [[nodiscard]] virtual bool Reset(void) override;
        [[nodiscard]] virtual const ParserNode* GetValue(void) const override;
        [[nodiscard]] virtual size_t GetSize(void) const override;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
        [[nodiscard]] bool operator==(const ParserNode& other) const;
        [[nodiscard]] bool operator!=(const ParserNode& other) const;

        private:
        String value;
        Array<ParserNode> children;
        size_t type;
    };
}

#endif