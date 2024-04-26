#ifndef Level_H
#define Level_H
#include "LevelStatus.hpp"
#include "Entity.hpp"

template <typename T>
struct Level {
    Level(Object<T> target_, std::vector<Object<T>> walls_, std::vector<Entity<T>> entities_) {
        target = target_;
        walls = walls_;
        entities = entities_;
    }
    LevelStatus Update(Second<T> time, Object<T>& player, Matrix<T> moveVector) {
        const Matrix<T> prev = player.cuboid.position;
        player.cuboid.position += moveVector * (player.velocity * time).GetValue();
        if (player.cuboid.CollidesWith(target.cuboid)) return LevelStatus::Victory;
        for (const Object<T>& wall : walls) {
            if (player.cuboid.CollidesWith(wall.cuboid)) {
                player.cuboid.position = prev;
                break;
            }
        }
        for (const Entity<T>& entity : entities)
            if (player.cuboid.CollidesWith(entity.cuboid)) return LevelStatus::GameOver;
        for (size_t i = 0; i < entities.size(); i++) {
            const Matrix<T> prev = entities.at(i).cuboid.position;
            entities.at(i).Update(time, player);
            bool found = false;
            for (size_t j = i + 1; j < entities.size() - 1 && !found; j++)
                if (entities.at(i).cuboid.CollidesWith(entities.at(j).cuboid)) found = true;
            for (size_t j = 0; j < walls.size() && !found; j++)
                if (entities.at(i).cuboid.CollidesWith(walls.at(j).cuboid)) found = true;
            if (found) entities.at(i).cuboid.position = prev;
        }
        return LevelStatus::Normal;
    }
    void Draw(Renderer& renderer) const {
        for (const Object<T>& wall : walls)
            wall.Draw(renderer);
        for (const Entity<T>& entity : entities)
            entity.Draw(renderer);
        target.Draw(renderer);
    }

    Object<T> target;
    std::vector<Object<T>> walls;
    std::vector<Entity<T>> entities;
};

#endif