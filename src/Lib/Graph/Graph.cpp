#include "Graph.hpp"
#include "../String.hpp"

namespace MathLib {
    Graph::Graph(void) : edges() {
        EmptyBenchmark
    }
    Graph::Graph(const Sequence<Edge>& edges) : edges(CollectionToArray<Edge>(edges)) {
        EmptyBenchmark
    }
    Edge Graph::At(size_t edge) const {
        StartAndReturnFromBenchmark(edges.At(edge));
    }
    Edge& Graph::At(size_t edge) {
        StartAndReturnFromBenchmark(edges.At(edge));
    }
    bool Graph::Add(const Edge& edge) {
        StartAndReturnFromBenchmark(edges.Add(edge));
    }
    bool Graph::Add(const Path& path) {
        StartBenchmark
        const size_t size = path.GetSize();
        if (size <= 1) ReturnFromBenchmark(false);
        for (size_t i = 1; i < size; i++)
            if (!edges.Add(Edge(path.At(i - 1), path.At(i)))) ReturnFromBenchmark(false);
        ReturnFromBenchmark(true);
    }
    EdgeDegree Graph::GetDegree(size_t edge) const {
        StartBenchmark
        EdgeDegree ret;
        for (const Edge& e : edges) {
            if (e.to == edge) ret.in++;
            if (e.from == edge) ret.out++;
        }
        ReturnFromBenchmark(ret);
    }
    Array<Path> Graph::GetPaths(const Edge& edge) const {
        StartBenchmark
        Array<Path> ret = MakeArray<Path>(Path());
        if (!ret.At(0).Add(edge.from)) ReturnFromBenchmark(Array<Path>());
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
                            if (!tmp.Add(e.to) || tmp.IsCyclic()) ReturnFromBenchmark(Array<Path>());
                        }
                        if (!buffer.Add(tmp)) ReturnFromBenchmark(Array<Path>());
                    }
                }
                if (add && !buffer.Add(path)) ReturnFromBenchmark(Array<Path>());
            }
            if (exit) ReturnFromBenchmark(buffer);
            ret = buffer;
            if (!buffer.Reset()) ReturnFromBenchmark(Array<Path>());
        }
    }
    Array<size_t> Graph::GetConnections(size_t edge, bool to) const {
        StartBenchmark
        Array<size_t> ret;
        for (const Edge& e : edges) {
            if (to) {
                if (e.to == edge && !ret.Add(e.from)) ReturnFromBenchmark(Array<size_t>());
            }
            else if (e.from == edge && !ret.Add(e.to)) ReturnFromBenchmark(Array<size_t>());
        }
        ReturnFromBenchmark(ret);
    }
    Array<size_t> Graph::GetEdges(size_t edge, bool to) const {
        StartBenchmark
        Array<size_t> ret;
        const size_t size = edges.GetSize();
        for (size_t i = 0; i < size; i++) {
            if (to) {
                if (edges.At(i).to == edge && !ret.Add(i)) ReturnFromBenchmark(Array<size_t>());
            }
            else if (edges.At(i).from == edge && !ret.Add(i)) ReturnFromBenchmark(Array<size_t>());
        }
        ReturnFromBenchmark(ret);
    }
    String Graph::ToString(const Sequence<char>& padding) const {
        StartBenchmark
        const String padd = CollectionToString(padding);
        String ret = padd + "digraph MathLib {\n";
        for (const Edge& edge : edges) ret += edge.ToString(padd + '\t') + '\n';
        ReturnFromBenchmark(ret + padd + '}');
    }
}