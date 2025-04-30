#ifndef MathLib_Graph_NetworkElement_H
#define MathLib_Graph_NetworkElement_H
#include "../Typedefs.hpp"

namespace MathLib {
    struct NetworkElement {
        size_t node;
        size_t link;
    
        NetworkElement(void) : node(SIZE_MAX), link(SIZE_MAX) {
            EmptyBenchmark
        }
        NetworkElement(size_t node, size_t link) : node(node), link(link) {
            EmptyBenchmark
        }
        [[nodiscard]] bool operator==(const NetworkElement& other) const {
            StartAndReturnFromBenchmark(node == other.node && link == other.link);
        }
        [[nodiscard]] bool operator!=(const NetworkElement& other) const {
            StartAndReturnFromBenchmark(!(*this == other));
        }
    };
}

#endif