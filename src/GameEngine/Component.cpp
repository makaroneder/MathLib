#include "Component.hpp"

Component::Component(UpdateFunc u, DrawFunc d) : update(u), draw(d) {}
Component::~Component(void) {}