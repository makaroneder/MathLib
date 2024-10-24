#ifndef Player_H
#define Player_H
#include "AbsoluteDialogIndex.hpp"
#include "Statistic.hpp"
#include "Entity.hpp"

template <typename T>
struct Player : Entity<T> {
    size_t statistics[(size_t)Statistic::StatisticCount];
    Array<AbsoluteDialogIndex> heardDialogs;

    Player(DummyRenderer& image, size_t health, const Matrix<T>& position) : Entity<T>(image, health, position) {}
    bool AddHeardDialog(const AbsoluteDialogIndex& dialog) {
        for (const AbsoluteDialogIndex& heardDialog : heardDialogs)
            if (heardDialog == dialog) return true;
        return heardDialogs.Add(dialog);
    }
    /// @brief Saves data
    /// @param file File to save data into
    /// @return Status
    virtual bool Save(Writeable& file) const override {
        if (!Entity<T>::Save(file) || !file.WriteBuffer(statistics, SizeOfArray(statistics) * sizeof(size_t)) || !file.Write<size_t>(heardDialogs.GetSize())) return false;
        for (const AbsoluteDialogIndex& dialog : heardDialogs)
            if (!dialog.Save(file)) return false;
        return true;
    }
    /// @brief Loads data
    /// @param file File to load data from
    /// @return Status
    virtual bool Load(Readable& file) override {
        size_t tmp = 0;
        if (!Entity<T>::Load(file) || !file.ReadBuffer(statistics, SizeOfArray(statistics) * sizeof(size_t)) || !file.Read<size_t>(tmp)) return false;
        heardDialogs = Array<AbsoluteDialogIndex>(tmp);
        for (AbsoluteDialogIndex& dialog : heardDialogs)
            if (!dialog.Load(file)) return false;
        return true;
    }
};

#endif