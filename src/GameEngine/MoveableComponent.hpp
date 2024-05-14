#ifndef MoveableComponent_H
#define MoveableComponent_H
#include "Component.hpp"

struct MoveableComponent : Component {
    MoveableComponent(UpdateFunc u, DrawFunc d, Matrix<num_t> pos, Matrix<num_t> rot);
    void Move(Matrix<num_t> dist);

    Matrix<num_t> position;
    Matrix<num_t> rotation;
};

#endif