#ifndef Player_H
#define Player_H
#include "GameStrategy.hpp"
#include <Interfaces/Printable.hpp>
#include <String.hpp>
#include <Host.hpp>

template <typename T>
struct Player : GameStrategy, MathLib::Printable {
    Player(void) : strategy(nullptr), score(T()), prevMove(SIZE_MAX) {}
    Player(GameStrategy* strategy, const T& score) : strategy(strategy), score(score), prevMove(SIZE_MAX) {}
    virtual ~Player(void) override {
        delete strategy;
    }
    [[nodiscard]] T GetScore(void) const {
        return score;
    }
    void ChangeScore(const T& add) {
        score += add;
    }
    [[nodiscard]] size_t GetPreviousMove(void) const {
        return prevMove;
    }
    void SetPreviousMove(size_t previousMove) {
        prevMove = previousMove;
    }
    [[nodiscard]] virtual size_t GetMove(size_t prevEnemyMove) override {
        prevMove = strategy->GetMove(prevEnemyMove);
        return prevMove;
    }
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override {
        MathLib::String ret = "{\n";
        ret += MathLib::CollectionToString(padding) + "\tScore: " + MathLib::ToString(score) + '\n';
        ret += MathLib::CollectionToString(padding) + "\tPrevious move: " + (prevMove == SIZE_MAX ? "None" : MathLib::ToString(prevMove, 10)) + '\n';
        return ret + padding + '}';
    }

    private:
    GameStrategy* strategy;
    T score;
    size_t prevMove;
};

#endif