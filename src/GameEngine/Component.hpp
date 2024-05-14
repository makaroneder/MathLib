#ifndef Component_H
#define Component_H
#include "UpdateData.hpp"
#include <Renderer.hpp>

enum class ComponentState {
    Normal,
    Error,
    Destroy,
};
struct Component;
using UpdateFunc = std::function<ComponentState(Component& self, UpdateData data)>;
using DrawFunc = std::function<ComponentState(const Component& self, Renderer& renderer)>;
struct Component {
    Component(UpdateFunc u, DrawFunc d);
    virtual ~Component(void);

    UpdateFunc update;
    DrawFunc draw;
};

#endif