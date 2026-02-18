#ifndef MathLib_Graph_H
#define MathLib_Graph_H
#include "Path.hpp"
#include "EdgeDegree.hpp"

namespace MathLib {
    struct Graph : Printable {
        Graph(void);
        Graph(const Sequence<Edge>& edges);
        [[nodiscard]] Edge At(size_t edge) const;
        [[nodiscard]] Edge& At(size_t edge);
        [[nodiscard]] bool Add(const Edge& edge);
        [[nodiscard]] bool Add(const Path& path);
        [[nodiscard]] EdgeDegree GetDegree(size_t edge) const;
        [[nodiscard]] Array<Path> GetPaths(const Edge& edge) const;
        [[nodiscard]] Array<size_t> GetConnections(size_t edge, bool to) const;
        [[nodiscard]] Array<size_t> GetEdges(size_t edge, bool to) const;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;

        private:
        Array<Edge> edges;
    };
}

#endif