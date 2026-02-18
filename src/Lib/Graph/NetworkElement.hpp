#ifndef MathLib_Graph_NetworkElement_H
#define MathLib_Graph_NetworkElement_H
#include "../Interfaces/Comparable.hpp"

namespace MathLib {
    struct NetworkElement;
    struct NetworkElement : MathLib::Comparable<NetworkElement> {
        size_t node;
        size_t link;

        NetworkElement(void);
        NetworkElement(size_t node, size_t link);

        protected:
        [[nodiscard]] virtual bool Equals(const NetworkElement& other) const override;
    };
}

#endif