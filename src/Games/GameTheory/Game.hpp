#ifndef Game_H
#define Game_H
#include "Player.hpp"
#include "Tensor.hpp"

template <typename T>
struct Game : MathLib::Printable {
    Game(const MathLib::Sequence<Player<T>*>& players, const Tensor<T>& payoffs) : players(MathLib::CollectionToArray(players)), payoffs(payoffs) {}
    virtual ~Game(void) override {
        for (Player<T>*& player : players) delete player;
    }
    void Update(size_t move) {
        MathLib::Array<size_t> moves = MathLib::Array<size_t>(players.GetSize() + 1);
        moves.At(0) = move;
        for (size_t i = 1; i < players.GetSize(); i++)
            moves.At(i) = players.At(i)->GetMove(players.At(0)->GetPreviousMove());
        players.At(0)->SetPreviousMove(move);
        for (size_t i = 0; i < players.GetSize(); i++) {
            moves.At(moves.GetSize() - 1) = i;
            players.At(i)->ChangeScore(payoffs.At(moves));
        }
    }
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override {
        MathLib::String ret = "{\n";
        ret += MathLib::CollectionToString(padding) + "\tPayoffs: " + payoffs.ToString() + '\n';
        for (size_t i = 0; i < players.GetSize(); i++)
            ret += MathLib::CollectionToString(padding) + "\tPlayer " + MathLib::ToString(i, 10) + ": " + players.At(i)->ToString(MathLib::CollectionToString(padding) + '\t') + '\n';
        return ret + padding + '}';
    }

    private:
    MathLib::Array<Player<T>*> players;
    Tensor<T> payoffs;
};

#endif