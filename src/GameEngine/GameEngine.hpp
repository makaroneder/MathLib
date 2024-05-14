#ifndef GameEngine_H
#define GameEngine_H
#include "Component.hpp"

struct GameEngine {
    GameEngine(Renderer& r);
    ~GameEngine(void);
    bool Run(void);
    void AddComponent(Component* component);
    Second<num_t> GetTime(void) const;

    private:
    bool HandleComponentState(ComponentState state, Component*& component);

    Renderer& renderer;
    std::vector<Component*> components;
    Event event;
};

#endif