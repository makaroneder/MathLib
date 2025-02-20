#include "TriggerAction.hpp"
#include "ConstantStrategy.hpp"

TriggerAction::TriggerAction(GameStrategy* strategy, const MathLib::Array<size_t>& triggers) : strategy(strategy), triggers(triggers) {}
TriggerAction::~TriggerAction(void) {
    delete strategy;
}
size_t TriggerAction::GetMove(size_t prevEnemyMove) {
    return strategy->GetMove(prevEnemyMove);
}
bool TriggerAction::Trigger(size_t move) const {
    return triggers.Contains(move);
}