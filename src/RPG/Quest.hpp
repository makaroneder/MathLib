#ifndef Quest_H
#define Quest_H
#include "QuestStep.hpp"

struct Quest : MathLib::Printable {
    Quest(const MathLib::String& name, const MathLib::String& description, const MathLib::Array<QuestStep>& steps);
    [[nodiscard]] bool IsHidden(void) const;
    template <typename T>
    void Check(const Player<T>& player) {
        for (; step < steps.GetSize(); step++)
            if (!steps.At(step).Check(player)) break;
    }
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::String& padding = "") const override;

    private:
    MathLib::String name;
    MathLib::String description;
    MathLib::Array<QuestStep> steps;
    size_t step;
};

#endif