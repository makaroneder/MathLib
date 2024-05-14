#include "GameEngine.hpp"
#include "TextComponent.hpp"
#include "LineShapeComponent.hpp"
#include <Geometry/Cuboid.hpp>
#include <SDL2.cpp>
#include <iostream>

/// @brief Creates update function for enemy
/// @param target Target of the enemy
/// @param gameOver Game state
/// @return Update function
UpdateFunc CreateEnemyUpdate(LineShapeComponent*& target, bool& gameOver) {
    return [&target, &gameOver](Component& self_, UpdateData data) -> ComponentState {
        LineShapeComponent& self = (LineShapeComponent&)self_;
        self.position += (target->position - self.position).Normalize() * data.deltaTime.GetValue();
        if (self.GetShape<Cuboid<num_t>>().CollidesWith(target->GetShape<Cuboid<num_t>>())) gameOver = true;
        return ComponentState::Normal;
    };
}
/// @brief Creates update function for collectable object
/// @param collector Component which can collect this object
/// @param totalScore Total score
/// @param score Score which this object will add
/// @param start Start of spawn area
/// @param end End of spawn area
/// @param add Addition to spawn location
/// @return Update function
UpdateFunc CreateCollectableUpdate(LineShapeComponent*& collector, Scalar<size_t>& totalScore, size_t score, Matrix<num_t> start, Matrix<num_t> end, Matrix<num_t> add) {
    return [&collector, &totalScore, score, start, end, add](Component& self_, UpdateData) -> ComponentState {
        LineShapeComponent& self = (LineShapeComponent&)self_;
        if (self.GetShape<Cuboid<num_t>>().CollidesWith(collector->GetShape<Cuboid<num_t>>())) {
            totalScore += score;
            num_t& x = GetX(self.position);
            num_t& y = GetY(self.position);
            const num_t save = x;
            x = y + GetX(add);
            y = save + GetY(add);
            if (x < GetX(start) || x > GetX(end) || y < GetY(start) || y > GetY(end)) {
                x = GetX(add);
                y = GetY(add);
            }
        }
        return ComponentState::Normal;
    };
}
/// @brief Entry point for this program
/// @return Status
int main(void) {
    try {
        SDL2Renderer renderer = SDL2Renderer("Game Engine", 800, 800);
        GameEngine engine = GameEngine(renderer);
        Matrix<num_t> playerDirection = CreateVector<num_t>(0, 0, 0);
        bool gameOver = false;
        Second<num_t> time = Second<num_t>(NAN);
        Scalar<size_t> score = 0;
        LineShapeComponent* player = new LineShapeComponent([&playerDirection, &gameOver](Component& self, UpdateData data) -> ComponentState {
            if (!gameOver && (data.event.type == Event::Type::KeyPressed || data.event.type == Event::Type::KeyReleased)) {
                const bool pressed = data.event.type == Event::Type::KeyPressed;
                switch (data.event.data.key) {
                    case 'w': {
                        GetY(playerDirection) = pressed;
                        break;
                    }
                    case 's': {
                        GetY(playerDirection) = -pressed;
                        break;
                    }
                    case 'a': {
                        GetX(playerDirection) = -pressed;
                        break;
                    }
                    case 'd': {
                        GetX(playerDirection) = pressed;
                        break;
                    }
                }
                ((LineShapeComponent&)self).Move(playerDirection * 10 * data.deltaTime.GetValue());
            }
            return ComponentState::Normal;
        }, CreateVector<num_t>(0, 0, 0), new Cuboid<num_t>(CreateVector<num_t>(0, 0, 0), CreateVector<num_t>(100 / renderer.pointMultiplier, 100 / renderer.pointMultiplier, 1 / renderer.pointMultiplier)), UINT32_MAX);
        engine.AddComponent(player);
        engine.AddComponent(new LineShapeComponent(CreateEnemyUpdate(player, gameOver), CreateVector<num_t>(0, 0, 0), new Cuboid<num_t>(CreateVector<num_t>(0, 4, 0), CreateVector<num_t>(50 / renderer.pointMultiplier, 50 / renderer.pointMultiplier, 1 / renderer.pointMultiplier)), 0xff0000ff));
        engine.AddComponent(new LineShapeComponent(CreateCollectableUpdate(player, score, 10, renderer.GetStart<num_t>(), renderer.GetEnd<num_t>(), CreateVector<num_t>(-2, 1, 0)), CreateVector<num_t>(0, 0, 0), new Cuboid<num_t>(CreateVector<num_t>(0, 4, 0), CreateVector<num_t>(50 / renderer.pointMultiplier, 50 / renderer.pointMultiplier, 1 / renderer.pointMultiplier)), 0x00ff00ff));
        engine.AddComponent(new LineShapeComponent(CreateCollectableUpdate(player, score, 10, renderer.GetStart<num_t>(), renderer.GetEnd<num_t>(), CreateVector<num_t>(1, -2, 0)), CreateVector<num_t>(0, 0, 0), new Cuboid<num_t>(CreateVector<num_t>(0, -4, 0), CreateVector<num_t>(50 / renderer.pointMultiplier, 50 / renderer.pointMultiplier, 1 / renderer.pointMultiplier)), 0x00ff00ff));
        engine.AddComponent(new TextComponent([&gameOver, &time, &score](Component& self_, UpdateData data) -> ComponentState {
            if (gameOver) {
                TextComponent& self = (TextComponent&)self_;
                if (std::isnan(time.GetValue())) {
                    time = data.deltaTime * data.iteration;
                    score += (size_t)time.GetValue() * 10;
                    self.text = {
                        "You survived for " + time.ToString(),
                        "Score: " + score.ToString(),
                    };
                }
                GetY(self.position) -= data.deltaTime.GetValue();
                if (GetY(self.position) < -4) GetY(self.position) = 4;
            }
            return ComponentState::Normal;
        }, CreateVector<num_t>(0, 3, 0), CreateVector<num_t>(0, 0, 0), { "RUN", }, &_binary_src_Lib_zap_light16_psf_start, CreateVector<size_t>(2, 2, 1), 0xff0000ff));
        if (!engine.Run()) throw std::runtime_error("Engine failed to run");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}