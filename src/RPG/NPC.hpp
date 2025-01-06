#ifndef NPC_H
#define NPC_H
#include "Dialog.hpp"
#include "NPCTrigger.hpp"

template <typename T>
struct NPC : Entity<T> {
    NPC(MathLib::DummyRenderer& image, size_t health, const MathLib::Matrix<T>& position, const MathLib::Array<Dialog>& dialogs, const MathLib::Array<NPCTrigger>& triggers) : Entity<T>(image, health, position), dialog(0), dialogs(dialogs), triggers(triggers) {}
    [[nodiscard]] MathLib::String GetDialog(const Player<T>& player) const {
        return dialogs.At(dialog).ToString<T>(player);
    }
    [[nodiscard]] NPCTrigger SetOption(const Player<T>& player, size_t option) {
        const MathLib::Expected<size_t> tmp = dialogs.At(dialog).GetNextDialog<T>(player, option);
        if (!tmp.HasValue()) return NPCTrigger(NPCTrigger::Type::None, SIZE_MAX);
        dialog = tmp.Get();
        for (const NPCTrigger& trigger : triggers)
            if (trigger.dialog == dialog) return trigger;
        return NPCTrigger(NPCTrigger::Type::None, dialog);
    }
    [[nodiscard]] size_t GetDialogIndex(void) const {
        return dialog;
    }
    /// @brief Saves data
    /// @param file File to save data into
    /// @return Status
    [[nodiscard]] virtual bool Save(MathLib::Writeable& file) const override {
        return Entity<T>::Save(file) && file.Write<size_t>(dialog);
    }
    /// @brief Loads data
    /// @param file File to load data from
    /// @return Status
    [[nodiscard]] virtual bool Load(MathLib::Readable& file) override {
        return Entity<T>::Load(file) && file.Read<size_t>(dialog);
    }

    private:
    size_t dialog;
    MathLib::Array<Dialog> dialogs;
    MathLib::Array<NPCTrigger> triggers;
};

#endif