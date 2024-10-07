#ifndef Entity_H
#define Entity_H
#include <DummyRenderer.hpp>

template <typename T>
struct Entity : Saveable {
    DummyRenderer& image;
    size_t health;
    Matrix<T> position;

    Entity(DummyRenderer& image, size_t health, const Matrix<T>& position) : image(image), health(health), position(position) {}
    /// @brief Saves data
    /// @param file File to save data into
    /// @return Status
    virtual bool Save(Writeable& file) const override {
        return file.Write<size_t>(health) && position.Save(file);
    }
    /// @brief Loads data
    /// @param file File to load data from
    /// @return Status
    virtual bool Load(Readable& file) override {
        return file.Read<size_t>(health) && position.Load(file);
    }
    bool Draw(Renderer& renderer) const {
        const Matrix<T> rotation = CreateVector<T>(0, 0, 0);
        #ifdef ShowHitbox
        renderer.DrawCircle2D<T>(position, hitboxRadius, 0x00ff00ff);
        #endif
        image.position = position;
        return renderer.DrawImage<T>(image, rotation);
    }
    bool Collides(const Matrix<T>& pos) const {
        return (pos - position).GetLength() <= hitboxRadius * 2;
    }

    private:
    static constexpr T hitboxRadius = 0.25;
};

#endif