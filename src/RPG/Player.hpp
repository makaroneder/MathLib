#ifndef Player_H
#define Player_H
#include "Statistic.hpp"
#include "Entity.hpp"

template <typename T>
struct Player : Entity<T> {
    size_t statistics[(size_t)Statistic::StatisticCount];

    Player(DummyRenderer& image, size_t health, const Matrix<T>& position) : Entity<T>(image, health, position) {}
    /// @brief Saves data
    /// @param file File to save data into
    /// @return Status
    virtual bool Save(Writeable& file) const override {
        return Entity<T>::Save(file) && file.WriteBuffer(statistics, SizeOfArray(statistics) * sizeof(size_t));
    }
    /// @brief Loads data
    /// @param file File to load data from
    /// @return Status
    virtual bool Load(Readable& file) override {
        return Entity<T>::Load(file) && file.ReadBuffer(statistics, SizeOfArray(statistics) * sizeof(size_t));
    }
};

#endif