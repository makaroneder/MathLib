#ifndef DialogOption_H
#define DialogOption_H
#include "Player.hpp"

struct DialogOption : MathLib::Allocatable {
    DialogOption(const MathLib::String& text, size_t nextDialog);
    DialogOption(Statistic requiredStatistic, size_t requiredStatisticValue, const MathLib::String& text, const MathLib::String& alternativeText, size_t nextDialog, size_t alternativeNextDialog);
    template<typename T>
    size_t GetNextDialog(const Player<T>& player) {
        return player.statistics[(size_t)requiredStatistic] < requiredStatisticValue ? alternativeNextDialog : nextDialog;
    }
    template<typename T>
    MathLib::String ToString(const Player<T>& player) const {
        return player.statistics[(size_t)requiredStatistic] < requiredStatisticValue ? alternativeText : text;
    }

    private:
    Statistic requiredStatistic;
    size_t requiredStatisticValue;
    MathLib::String text;
    MathLib::String alternativeText;
    size_t nextDialog;
    size_t alternativeNextDialog;
};

#endif