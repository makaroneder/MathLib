#ifndef MathLib_Graph_NetworkElement_H
#define MathLib_Graph_NetworkElement_H
#include <stddef.h>
#include <stdint.h>

namespace MathLib {
    struct NetworkElement {
        size_t node;
        size_t link;
    
        NetworkElement(void) : node(SIZE_MAX), link(SIZE_MAX) {}
        NetworkElement(size_t node, size_t link) : node(node), link(link) {}
        [[nodiscard]] bool operator==(const NetworkElement& other) const {
            return node == other.node && link == other.link;
        }
        [[nodiscard]] bool operator!=(const NetworkElement& other) const {
            return !(*this == other);
        }
    };
}

#endif