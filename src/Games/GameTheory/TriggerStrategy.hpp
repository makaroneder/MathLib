#ifndef TriggerStrategy_H
#define TriggerStrategy_H
#include "TriggerAction.hpp"

struct TriggerStrategy : GameStrategy {
    TriggerStrategy(const MathLib::Array<TriggerAction*>& actions);
    virtual ~TriggerStrategy(void) override;
    [[nodiscard]] virtual size_t GetMove(size_t prevEnemyMove) override;

    private:
    MathLib::Array<TriggerAction*> actions;
    size_t action;
};

#endif