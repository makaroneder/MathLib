#include "GameEngine.hpp"

GameEngine::GameEngine(Renderer& r) : renderer(r) {}
GameEngine::~GameEngine(void) {
    for (Component*& component : components)
        if (component) delete component;
}
void GameEngine::AddComponent(Component* component) {
    for (Component*& comp : components) {
        if (!comp) {
            comp = component;
            return;
        }
    }
    components.push_back(component);
}
Second<num_t> GameEngine::GetTime(void) const {
    return (num_t)clock() / CLOCKS_PER_SEC;
}
bool GameEngine::HandleComponentState(ComponentState state, Component*& component) {
    if (state == ComponentState::Destroy) {
        delete component;
        component = nullptr;
    }
    return state != ComponentState::Error;
}
bool GameEngine::Run(void) {
    Second<num_t> prevTime = GetTime();
    for (size_t i = 0; true; i++) {
        renderer.Fill(0x000000ff);
        const Second<num_t> currTime = GetTime();
        const UpdateData data = UpdateData(currTime - prevTime, i, event);
        prevTime = currTime;
        for (Component*& component : components) {
            if (component && !HandleComponentState(component->update(*component, data), component)) return false;
            if (component && !HandleComponentState(component->draw(*component, renderer), component)) return false;
        }
        if (!renderer.Update()) return false;
        if (event.type == Event::Type::Quit) break;
        event = renderer.GetEvent();
    }
    return true;
}