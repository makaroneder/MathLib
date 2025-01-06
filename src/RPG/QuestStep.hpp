#ifndef QuestStep_H
#define QuestStep_H
#include "Player.hpp"

struct QuestStep : MathLib::Allocatable {
    QuestStep(const MathLib::String& objective, const AbsoluteDialogIndex& dialog, bool hidden);
    [[nodiscard]] MathLib::String GetObjective(void) const;
    [[nodiscard]] bool IsHidden(void) const;
    template <typename T>
    [[nodiscard]] bool Check(const Player<T>& player) const {
        for (const AbsoluteDialogIndex& playerDialog : player.heardDialogs)
            if (dialog == playerDialog) return true;
        return false;
    }

    private:
    MathLib::String objective;
    AbsoluteDialogIndex dialog;
    bool hidden;
};

#endif