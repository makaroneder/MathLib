#include "NetworkElement.hpp"
#include <stdint.h>

namespace MathLib {
    NetworkElement::NetworkElement(void) : node(SIZE_MAX), link(SIZE_MAX) {}
    NetworkElement::NetworkElement(size_t node, size_t link) : node(node), link(link) {}
    bool NetworkElement::Equals(const NetworkElement& other) const {
        return node == other.node && link == other.link;
    }
}