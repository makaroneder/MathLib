#ifndef MathLib_Graph_Path_H
#define MathLib_Graph_Path_H
#include "Edge.hpp"

namespace MathLib {
    struct Path : Collection<size_t>, Printable {
        Path(void);
        Path(const Sequence<size_t>& vertices);
        [[nodiscard]] virtual size_t GetSize(void) const override;
        [[nodiscard]] virtual bool Add(const size_t& value) override;
        [[nodiscard]] virtual bool Reset(void) override;
        [[nodiscard]] virtual size_t* GetValue(void) override;
        [[nodiscard]] virtual const size_t* GetValue(void) const override;
        [[nodiscard]] size_t GetEnd(void) const;
        [[nodiscard]] Edge GetEdge(void) const;
        [[nodiscard]] bool IsCyclic(void) const;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;

        private:
        Array<size_t> vertices;
    };
}

#endif