#include "TriggerStrategy.hpp"

TriggerStrategy::TriggerStrategy(const MathLib::Array<TriggerAction*>& actions) : actions(actions), action(0) {}
TriggerStrategy::~TriggerStrategy(void) {
    for (TriggerAction*& action : actions) delete action;
}
size_t TriggerStrategy::GetMove(size_t prevEnemyMove) {
    for (size_t i = 0; i < actions.GetSize(); i++) {
        if (actions[i]->Trigger(prevEnemyMove)) {
            action = i;
            break;
        }
    }
    return actions[action]->GetMove(prevEnemyMove);
}