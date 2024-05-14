#include "RendererComponent.hpp"

RendererComponent::RendererComponent(UpdateFunc u, Matrix<num_t> rot, DummyRenderer dummy) : MoveableComponent(u, [](const Component& self_, Renderer& renderer) -> ComponentState {
    const RendererComponent& self = (const RendererComponent&)self_;
    DummyRenderer dummy = self.renderer;
    dummy.position = self.position;
    return renderer.DrawImage<num_t>(dummy, self.rotation) ? ComponentState::Normal : ComponentState::Error;
}, dummy.position, rot), renderer(dummy) {}