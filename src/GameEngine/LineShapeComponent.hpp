#ifndef LineShapeComponent_H
#define LineShapeComponent_H
#include "MoveableComponent.hpp"

struct LineShapeComponent : MoveableComponent {
    LineShapeComponent(UpdateFunc u, Matrix<num_t> rot, LineShape<num_t>* s, uint32_t c);
    ~LineShapeComponent(void);
    template <class T>
    T& GetShape(void) {
        return *(T*)shape;
    }
    uint32_t color;

    private:
    LineShape<num_t>* shape;
};

#endif