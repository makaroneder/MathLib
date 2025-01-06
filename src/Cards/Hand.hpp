#ifndef Hand_H
#define Hand_H
#include "CardIndex.hpp"

template <typename T>
struct Hand : MathLib::Printable {
    static constexpr size_t size = 5;
    enum class Type {
        HighCard = 1,
        Pair,
        TwoPair,
        ThreeOfAKind,
        Straight,
        Flush,
        FullHouse,
        FourOfAKind,
        StraightFlush,
    };
    Hand(const MathLib::Matrix<T>& position) : cards(MathLib::Array<CardIndex>(size)), position(position) {
        for (CardIndex& card : cards) card.selected = true;
    }
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::String& padding = "") const override {
        MathLib::String ret = padding + "{\n";
        for (const CardIndex& card : cards) ret += card.ToString(padding + '\t') + '\n';
        return ret + padding + '}';
    }
    [[nodiscard]] bool ShouldSwap(size_t i) {
        return cards.At(i).selected;
    }
    [[nodiscard]] CardIndex Swap(const CardIndex& card, size_t i) {
        const CardIndex ret = cards.At(i);
        cards.At(i) = card;
        return ret;
    }
    [[nodiscard]] bool IsFlush(void) const {
        const Card::Color color = cards.At(0).color;
        for (size_t i = 1; i < cards.GetSize(); i++)
            if (cards.At(i).color != color) return false;
        return true;
    }
    [[nodiscard]] bool IsStraight(void) const {
        const MathLib::Array<CardIndex> sorted = MathLib::BubbleSort<CardIndex>(cards);
        for (size_t i = 1; i < sorted.GetSize(); i++)
            if ((size_t)sorted.At(i - 1).type - (size_t)sorted.At(i).type != 1) return false;
        return true;
    }
    [[nodiscard]] MathLib::Array<size_t> GetEqualValueCount(void) const {
        MathLib::Array<size_t> ret = MathLib::Array<size_t>(1);
        ret.At(0) = 1;
        Card::Type prev = cards.At(0).type;
        for (size_t i = 1; i < cards.GetSize(); i++) {
            if (cards.At(i).type == prev) ret.At(ret.GetSize() - 1)++;
            else {
                prev = cards.At(i).type;
                if (!ret.Add(1)) return MathLib::Array<size_t>();
            }
        }
        return MathLib::BubbleSort<size_t>(ret);
    }
    [[nodiscard]] Type GetType(void) const {
        if (IsFlush() && IsStraight()) return Type::StraightFlush;
        else if (IsStraight()) return Type::Straight;
        else if (IsFlush()) return Type::Flush;
        else {
            const MathLib::Array<size_t> tmp = GetEqualValueCount();
            if (tmp.GetSize() == 4) return Type::Pair;
            else if (tmp.GetSize() == 3) {
                if (tmp.At(0) == 2) return Type::TwoPair;
                else if (tmp.At(0) == 3) return Type::ThreeOfAKind;
            }
            else if (tmp.GetSize() == 2) {
                if (tmp.At(0) == 3) return Type::FullHouse;
                else if (tmp.At(0) == 4) return Type::FourOfAKind;
            }
        }
        return Type::HighCard;
    }
    [[nodiscard]] size_t GetPoints(void) const {
        size_t points = 0;
        for (const CardIndex& card : cards) points += (size_t)card.type + 1;
        return points * (size_t)GetType();
    }
    [[nodiscard]] MathLib::Matrix<T> GetPosition(size_t card) const {
        MathLib::Matrix<T> pos = position;
        GetX(pos) -= (Card::width + Card::padding) * (card - (T)cards.GetSize() / 2 + 0.5);
        GetY(pos) += cards.At(card).selected * Card::padding * 5;
        return pos;
    }
    [[nodiscard]] bool Select(const MathLib::Matrix<T>& cursorPosition) {
        MathLib::Cuboid<T> cursor = MathLib::Cuboid<T>(cursorPosition, MathLib::CreateVector<T>(0.01, 0.01, 0.01));
        for (size_t i = 0; i < cards.GetSize(); i++) {
            if (Card::ToCuboid(GetPosition(i)).CollidesWith(cursor)) {
                cards.At(i).selected = !cards.At(i).selected;
                return true;
            }
        }
        return false;
    }
    void SelectAll(void) {
        for (CardIndex& card : cards) card.selected = true;
    }
    [[nodiscard]] bool Draw(MathLib::Renderer& renderer, const MathLib::Matrix<Card>& cardsData, const MathLib::Matrix<T>& rotation) const {
        for (size_t i = 0; i < cards.GetSize(); i++)
            if (!cardsData.At((size_t)cards.At(i).type, (size_t)cards.At(i).color).Draw(renderer, GetPosition(i), rotation)) return false;
        return true;
    }

    private:
    MathLib::Array<CardIndex> cards;
    MathLib::Matrix<T> position;
};

#endif