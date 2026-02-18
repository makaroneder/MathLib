#include "Graph.hpp"

namespace MathLib {
    Graph::Graph(void) : edges() {}
    Graph::Graph(const Sequence<Edge>& edges) : edges(CollectionToArray<Edge>(edges)) {}
    Edge Graph::At(size_t edge) const {
        return edges.At(edge);
    }
    Edge& Graph::At(size_t edge) {
        return edges.At(edge);
    }
    bool Graph::Add(const Edge& edge) {
        return edges.Add(edge);
    }
    bool Graph::Add(const Path& path) {
        const size_t size = path.GetSize();
        if (size <= 1) return false;
        for (size_t i = 1; i < size; i++)
            if (!edges.Add(Edge(path.AtUnsafe(i - 1), path.AtUnsafe(i)))) return false;
        return true;
    }
    EdgeDegree Graph::GetDegree(size_t edge) const {
        EdgeDegree ret;
        for (const Edge& e : edges) {
            if (e.to == edge) ret.in++;
            if (e.from == edge) ret.out++;
        }
        return ret;
    }
    Array<Path> Graph::GetPaths(const Edge& edge) const {
        Array<Path> ret = MakeArray<Path>(Path(MakeArray<size_t>(edge.from)));
        Array<Path> buffer;
        while (true) {
            bool exit = true;
            for (const Path& path : ret) {
                const size_t end = path.GetEnd();
                bool add = end == edge.to;
                for (const Edge& e : edges) {
                    if (end == e.from) {
                        add = false;
                        Path tmp = path;
                        if (end != edge.to) {
                            exit = false;
                            if (!tmp.Add(e.to) || tmp.IsCyclic()) return Array<Path>();
                        }
                        if (!buffer.Add(tmp)) return Array<Path>();
                    }
                }
                if (add && !buffer.Add(path)) return Array<Path>();
            }
            if (exit) return buffer;
            ret = buffer;
            if (!buffer.Reset()) return Array<Path>();
        }
    }
    Array<size_t> Graph::GetConnections(size_t edge, bool to) const {
        Array<size_t> ret;
        for (const Edge& e : edges) {
            if (to) {
                if (e.to == edge && !ret.Add(e.from)) return Array<size_t>();
            }
            else if (e.from == edge && !ret.Add(e.to)) return Array<size_t>();
        }
        return ret;
    }
    Array<size_t> Graph::GetEdges(size_t edge, bool to) const {
        Array<size_t> ret;
        const size_t size = edges.GetSize();
        for (size_t i = 0; i < size; i++) {
            if (to) {
                if (edges.AtUnsafe(i).to == edge && !ret.Add(i)) return Array<size_t>();
            }
            else if (edges.AtUnsafe(i).from == edge && !ret.Add(i)) return Array<size_t>();
        }
        return ret;
    }
    String Graph::ToString(const Sequence<char>& padding) const {
        const String padd = CollectionToString(padding);
        String ret = padd + "digraph MathLib {\n";
        for (const Edge& edge : edges) ret += edge.ToString(padd + '\t') + '\n';
        return ret + padd + '}';
    }
}