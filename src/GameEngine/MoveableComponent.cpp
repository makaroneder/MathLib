#include "MoveableComponent.hpp"

MoveableComponent::MoveableComponent(UpdateFunc u, DrawFunc d, Matrix<num_t> pos, Matrix<num_t> rot) : Component(u, d), position(pos), rotation(rot) {}
void MoveableComponent::Move(Matrix<num_t> dist) {
    position = RotateVector<num_t>(position + dist, position, rotation);
}