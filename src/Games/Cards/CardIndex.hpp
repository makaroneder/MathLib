#ifndef CardIndex_H
#define CardIndex_H
#include "Card.hpp"

struct CardIndex : MathLib::Orderable, MathLib::Printable {
    Card::Type type;
    Card::Color color;
    bool selected;

    CardIndex(Card::Type type = Card::Type::TypeCount, Card::Color color = Card::Color::ColorCount);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    protected:
    [[nodiscard]] virtual bool LessThanEqual(const MathLib::Orderable& other) const override;
};

#endif