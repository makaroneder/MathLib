#ifndef DialogOption_H
#define DialogOption_H
#include "Player.hpp"

struct DialogOption {
    DialogOption(const String& text, size_t nextDialog);
    DialogOption(Statistic requiredStatistic, size_t requiredStatisticValue, const String& text, const String& alternativeText, size_t nextDialog, size_t alternativeNextDialog);
    template<typename T>
    size_t GetNextDialog(const Player<T>& player) {
        return player.statistics[(size_t)requiredStatistic] < requiredStatisticValue ? alternativeNextDialog : nextDialog;
    }
    template<typename T>
    String ToString(const Player<T>& player) const {
        return player.statistics[(size_t)requiredStatistic] < requiredStatisticValue ? alternativeText : text;
    }

    private:
    Statistic requiredStatistic;
    size_t requiredStatisticValue;
    String text;
    String alternativeText;
    size_t nextDialog;
    size_t alternativeNextDialog;
};

#endif