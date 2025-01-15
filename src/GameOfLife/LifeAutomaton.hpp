#ifndef LifeAutomaton_H
#define LifeAutomaton_H
#include "LifeAutomatonRule.hpp"
#include <Renderer.hpp>

struct LifeAutomaton : MathLib::Printable {
    LifeAutomaton(size_t width, size_t height, const LifeAutomatonRule& rule);
    void Set(size_t x, size_t y, bool value);
    [[nodiscard]] uint8_t GetNeighbours(size_t x, size_t y) const;
    [[nodiscard]] LifeAutomaton Update(void) const;
    [[nodiscard]] MathLib::Image ToImage(uint32_t deadColor, uint32_t aliveColor) const;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::String& padding = "") const override;

    private:
    MathLib::Matrix<uint8_t> data;
    LifeAutomatonRule rule;
};

#endif