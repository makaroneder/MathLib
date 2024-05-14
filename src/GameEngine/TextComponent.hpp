#ifndef TextComponent_H
#define TextComponent_H
#include "MoveableComponent.hpp"

struct TextComponent : MoveableComponent {
    TextComponent(UpdateFunc u, Matrix<num_t> pos, Matrix<num_t> rot, std::vector<std::string> str, PSF1* f, Matrix<size_t> s, uint32_t c);

    std::vector<std::string> text;
    PSF1* font;
    Matrix<size_t> scale;
    uint32_t color;
};

#endif