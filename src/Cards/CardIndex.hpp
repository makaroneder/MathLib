#ifndef CardIndex_H
#define CardIndex_H
#include "Card.hpp"

struct CardIndex : MathLib::Printable {
    Card::Type type;
    Card::Color color;
    bool selected;

    CardIndex(Card::Type type = Card::Type::TypeCount, Card::Color color = Card::Color::ColorCount);
    bool operator<(const CardIndex& other) const;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::String& padding = "") const override;
};

#endif