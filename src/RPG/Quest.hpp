#ifndef Quest_H
#define Quest_H
#include "QuestStep.hpp"

struct Quest : Printable {
    Quest(const String& name, const String& description, const Array<QuestStep>& steps);
    bool IsHidden(void) const;
    template <typename T>
    bool Check(const Player<T>& player) {
        for (; step < steps.GetSize(); step++)
            if (!steps.At(step).Check(player)) return false;
        return true;
    }
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual String ToString(const String& padding = "") const override;

    private:
    String name;
    String description;
    Array<QuestStep> steps;
    size_t step;
};

#endif