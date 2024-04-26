#ifndef GameEngine_H
#define GameEngine_H
#include "Level.hpp"

template <typename T>
struct GameEngine {
    GameEngine(Object<T> p, std::vector<Level<T>> lvls) {
        player = p;
        levels = lvls;
        currentLevel = 0;
    }
    LevelStatus Update(Second<T> time, Matrix<num_t> moveVector) {
        if (currentLevel >= levels.size()) return LevelStatus::Error;
        const LevelStatus status = levels.at(currentLevel).Update(time, player, moveVector);
        if (status == LevelStatus::Victory && ++currentLevel != levels.size()) {
            player.cuboid.position = CreateVector<T>(0, 0, 0);
            return LevelStatus::Normal;
        }
        return status;
    }
    bool Draw(Renderer& renderer) const {
        if (currentLevel >= levels.size()) return false;
        levels.at(currentLevel).Draw(renderer);
        player.Draw(renderer);
    }

    private:
    Object<T> player;
    std::vector<Level<T>> levels;
    size_t currentLevel;
};

#endif