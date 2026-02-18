#ifndef MathLib_Graph_EdgeDegree_H
#define MathLib_Graph_EdgeDegree_H
#include <stddef.h>

namespace MathLib {
    struct EdgeDegree {
        size_t in;
        size_t out;

        EdgeDegree(void);
    };
}

#endif