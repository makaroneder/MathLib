#ifndef QuestStep_H
#define QuestStep_H
#include "Player.hpp"

struct QuestStep {
    QuestStep(const String& objective, const AbsoluteDialogIndex& dialog, bool hidden);
    String GetObjective(void) const;
    bool IsHidden(void) const;
    template <typename T>
    bool Check(const Player<T>& player) const {
        for (const AbsoluteDialogIndex& playerDialog : player.heardDialogs)
            if (dialog == playerDialog) return true;
        return false;
    }

    private:
    String objective;
    AbsoluteDialogIndex dialog;
    bool hidden;
};

#endif