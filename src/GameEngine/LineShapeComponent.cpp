#include "LineShapeComponent.hpp"

LineShapeComponent::LineShapeComponent(UpdateFunc u, Matrix<num_t> rot, LineShape<num_t>* s, uint32_t c) : MoveableComponent(u, [](const Component& self_, Renderer& renderer) -> ComponentState {
    const LineShapeComponent& self = (const LineShapeComponent&)self_;
    if (!self.shape) return ComponentState::Error;
    self.shape->position = self.position;
    renderer.DrawShape<num_t>(*self.shape, self.rotation, self.color);
    return ComponentState::Normal;
}, s->position, rot), color(c), shape(s) {}
LineShapeComponent::~LineShapeComponent(void) {
    delete shape;
}