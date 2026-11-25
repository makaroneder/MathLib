#include "Bullet.hpp"

Bullet::Bullet(void) : animation(), x(), y(), type(Type::None) {}
Bullet::Bullet(const MathLib::Video* video, float x, float y, Type type) : animation(video), x(x), y(y), type(type) {}
bool Bullet::IsValid(void) const {
    return type != Type::None;
}
bool Bullet::Draw(MathLib::Renderer& renderer, float offsetX, float offsetY, float deltaTime) {
    switch (type) {
        case Type::Up: {
            y -= speed * deltaTime;
            if (y - offsetY + animation.GetHeight() / 2 + renderer.GetHeight() / 2 < 0) return false;
            break;
        }
        case Type::Down: {
            y += speed * deltaTime;
            if (y - offsetY - animation.GetHeight() / 2 > renderer.GetHeight() / 2) return false;
            break;
        }
        case Type::Left: {
            x -= speed * deltaTime;
            if (x - offsetX + animation.GetWidth() / 2 + renderer.GetWidth() / 2 < 0) return false;
            break;
        }
        case Type::Right: {
            x += speed * deltaTime;
            if (x - offsetX - animation.GetWidth() / 2 > renderer.GetWidth() / 2) return false;
            break;
        }
        default: return true;
    }
    animation.Draw(renderer, renderer.GetWidth() / 2 + x - offsetX, renderer.GetHeight() / 2 + y - offsetY);
    animation.Update(deltaTime);
    return true;
}
bool Bullet::Equals(const Bullet& other) const {
    return type == other.type && x == other.x && y == other.y && animation == other.animation;
}