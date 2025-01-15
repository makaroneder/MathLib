#ifndef Entity_H
#define Entity_H
#include <DummyRenderer.hpp>

template <typename T>
struct Entity : MathLib::Saveable {
    MathLib::Image& image;
    size_t health;
    MathLib::Matrix<T> position;

    Entity(MathLib::Image& image, size_t health, const MathLib::Matrix<T>& position) : image(image), health(health), position(position) {}
    /// @brief Saves data
    /// @param file File to save data into
    /// @return Status
    [[nodiscard]] virtual bool Save(MathLib::Writable& file) const override {
        return file.Write<size_t>(health) && position.Save(file);
    }
    /// @brief Loads data
    /// @param file File to load data from
    /// @return Status
    [[nodiscard]] virtual bool Load(MathLib::Readable& file) override {
        return file.Read<size_t>(health) && position.Load(file);
    }
    void Draw(MathLib::Renderer& renderer) const {
        #ifdef ShowHitbox
        renderer.DrawCircle2D<T>(position, hitboxRadius, 0x00ff00ff);
        #endif
        renderer.DrawImage<T>(image, position);
    }
    [[nodiscard]] bool Collides(const MathLib::Matrix<T>& pos) const {
        return (pos - position).GetLength() <= hitboxRadius * 2;
    }

    private:
    static constexpr T hitboxRadius = 0.25;
};

#endif