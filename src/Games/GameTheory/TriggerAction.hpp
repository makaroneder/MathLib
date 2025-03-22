#ifndef TriggerAction_H
#define TriggerAction_H
#include "GameStrategy.hpp"

struct TriggerAction : GameStrategy {
    TriggerAction(GameStrategy* strategy, const MathLib::Sequence<size_t>& triggers);
    virtual ~TriggerAction(void) override;
    [[nodiscard]] virtual size_t GetMove(size_t prevEnemyMove) override;
    [[nodiscard]] bool Trigger(size_t move) const;

    private:
    GameStrategy* strategy;
    MathLib::Array<size_t> triggers;
};

#endif