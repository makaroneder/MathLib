#ifndef RendererComponent_H
#define RendererComponent_H
#include "MoveableComponent.hpp"
#include <DummyRenderer.hpp>

struct RendererComponent : MoveableComponent {
    RendererComponent(UpdateFunc u, Matrix<num_t> rot, DummyRenderer dummy);

    private:
    DummyRenderer renderer;
};

#endif