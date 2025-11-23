#ifndef MathLib_Graph_Network_H
#define MathLib_Graph_Network_H
#include "NetworkElement.hpp"
#include "Graph.hpp"

namespace MathLib {
    template <typename Node, typename Link>
    struct Network : Graph {
        Network(void) : Graph(), nodes(), links() {}
        Network(const Sequence<Edge>& edges, const Sequence<Node>& nodes, const Sequence<Link>& links) : Graph(edges), nodes(nodes), links(links) {}
        [[nodiscard]] size_t GetNodeCount(void) const {
            return nodes.GetSize();
        }
        [[nodiscard]] size_t GetLinkCount(void) const {
            return links.GetSize();
        }
        [[nodiscard]] Node GetNode(size_t node) const {
            return nodes.At(node);
        }
        [[nodiscard]] Node& GetNode(size_t node) {
            return nodes.At(node);
        }
        [[nodiscard]] Link GetLink(size_t link) const {
            return links.At(link);
        }
        [[nodiscard]] Link& GetLink(size_t link) {
            return links.At(link);
        }
        [[nodiscard]] bool ResetNodes(const Function<void, Node&>& func) {
            for (Node& node : nodes) func(node);
            return nodes.Reset();
        }
        [[nodiscard]] bool ResetLinks(const Function<void, Link&>& func) {
            for (Link& link : links) func(link);
            return links.Reset();
        }
        [[nodiscard]] size_t Add(const Node& node) {
            return nodes.Add(node) ? nodes.GetSize() - 1 : SIZE_MAX;
        }
        [[nodiscard]] bool Add(const Edge& edge, const Link& link) {
            return Graph::Add(edge) && links.Add(link);
        }
        [[nodiscard]] Array<NetworkElement> GetConnectionsAttributes(size_t edge, bool to) const {
            const Array<size_t> tmp = GetEdges(edge, to);
            const size_t size = tmp.GetSize();
            Array<NetworkElement> ret = Array<NetworkElement>(size);
            for (size_t i = 0; i < size; i++) {
                const size_t link = tmp.At(i);
                const Edge edge = At(link);
                ret.At(i) = NetworkElement(to ? edge.from : edge.to, link);
            }
            return ret;
        }
    
        private:
        Array<Node> nodes;
        Array<Link> links;
    };
}

#endif