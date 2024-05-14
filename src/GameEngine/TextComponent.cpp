#include "TextComponent.hpp"

TextComponent::TextComponent(UpdateFunc u, Matrix<num_t> pos, Matrix<num_t> rot, std::vector<std::string> str, PSF1* f, Matrix<size_t> s, uint32_t c) : MoveableComponent(u, [](const Component& self_, Renderer& renderer) -> ComponentState {
    const TextComponent& self = (const TextComponent&)self_;
    return renderer.Puts<num_t>(self.text, self.font, self.position, self.rotation, self.scale, self.color) ? ComponentState::Normal : ComponentState::Error;
}, pos, rot), text(str), font(f), scale(s), color(c) {}