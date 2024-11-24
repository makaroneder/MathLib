#ifndef Quest_H
#define Quest_H
#include "QuestStep.hpp"

struct Quest : MathLib::Printable {
    Quest(const MathLib::String& name, const MathLib::String& description, const MathLib::Array<QuestStep>& steps);
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
    virtual MathLib::String ToString(const MathLib::String& padding = "") const override;

    private:
    MathLib::String name;
    MathLib::String description;
    MathLib::Array<QuestStep> steps;
    size_t step;
};

#endif