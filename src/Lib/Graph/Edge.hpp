#ifndef MathLib_Graph_Edge_H
#define MathLib_Graph_Edge_H
#include "../Interfaces/Printable.hpp"

namespace MathLib {
    struct Edge : Printable {
        size_t from;
        size_t to;

        Edge(void);
        Edge(size_t from, size_t to);
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
        [[nodiscard]] bool operator==(const Edge& other) const;
        [[nodiscard]] bool operator!=(const Edge& other) const;
    };
}

#endif