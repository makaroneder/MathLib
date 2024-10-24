#ifndef NPC_H
#define NPC_H
#include "Dialog.hpp"

template <typename T>
struct NPC : Entity<T> {
    NPC(DummyRenderer& image, size_t health, const Matrix<T>& position, const Array<Dialog>& dialogs) : Entity<T>(image, health, position), dialog(0), dialogs(dialogs) {}
    String GetDialog(const Player<T>& player) const {
        return dialogs.At(dialog).ToString<T>(player);
    }
    bool SetOption(const Player<T>& player, size_t option) {
        const Expected<size_t> tmp = dialogs.At(dialog).GetNextDialog<T>(player, option);
        if (!tmp.HasValue()) return false;
        dialog = tmp.Get();
        return true;
    }
    size_t GetDialogIndex(void) const {
        return dialog;
    }
    /// @brief Saves data
    /// @param file File to save data into
    /// @return Status
    virtual bool Save(Writeable& file) const override {
        return Entity<T>::Save(file) && file.Write<size_t>(dialog);
    }
    /// @brief Loads data
    /// @param file File to load data from
    /// @return Status
    virtual bool Load(Readable& file) override {
        return Entity<T>::Load(file) && file.Read<size_t>(dialog);
    }

    private:
    size_t dialog;
    Array<Dialog> dialogs;
};

#endif